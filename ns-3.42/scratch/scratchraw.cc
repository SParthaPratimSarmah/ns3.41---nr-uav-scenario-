#include "ns3/antenna-module.h"
#include "ns3/applications-module.h"
#include "ns3/buildings-module.h"
#include "ns3/config-store-module.h"
#include "ns3/core-module.h"
#include "ns3/epc-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/net-device.h"
#include "ns3/netanim-module.h"
#include "ns3/network-module.h"
#include "ns3/node.h"
#include "ns3/nr-helper.h"
#include "ns3/nr-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;

NodeContainer fronthaulGnbContainer;
NodeContainer backhaulGnbContainer;
NodeContainer ueUavContainer;
double gNbHeight = 10.0;
double ueHeight = 1.5;

class UAV
{
  public:
    UAV()
    {
        id = ++uavCount;
        fronthaulGnb = CreateObject<Node>();
        backhaulGnb = CreateObject<Node>();
        ue = CreateObject<Node>();
        fronthaulGnbContainer.Add(fronthaulGnb);
        backhaulGnbContainer.Add(backhaulGnb);
        ueUavContainer.Add(ue);

        PointToPointHelper p2p;
        p2p.SetDeviceAttribute("DataRate", StringValue("10Gbps"));
        p2p.SetChannelAttribute("Delay", StringValue("2ms"));
        NetDeviceContainer p2pDevices1 = p2p.Install(fronthaulGnb, backhaulGnb);
        NetDeviceContainer p2pDevices2 = p2p.Install(backhaulGnb, ue);

        InternetStackHelper internet;
        internet.Install(fronthaulGnb);
        internet.Install(backhaulGnb);
        internet.Install(ue);

        Ipv4AddressHelper ipv4;
        ipv4.SetBase(baseNetwork, "255.255.255.0");
        ipv4.Assign(p2pDevices1);
        ipv4.Assign(p2pDevices2);

        IncrementBaseNetwork();
    }

    Ptr<Node> GetFronthaulGnb() const
    {
        return fronthaulGnb;
    }

    Ptr<Node> GetBackhaulGnb() const
    {
        return backhaulGnb;
    }

    Ptr<Node> GetUe() const
    {
        return ue;
    }

    uint32_t GetId() const
    {
        return id;
    }

    void PrintPositions()
    {
        Ptr<MobilityModel> fronthaulGnbMobility = fronthaulGnb->GetObject<MobilityModel>();
        Vector fronthaulGnbPosition = fronthaulGnbMobility->GetPosition();
        std::cout << "UAV ID: " << id << ", Fronthaul gNB Position: " << fronthaulGnbPosition
                  << std::endl;

        Ptr<MobilityModel> backhaulGnbMobility = backhaulGnb->GetObject<MobilityModel>();
        Vector backhaulGnbPosition = backhaulGnbMobility->GetPosition();
        std::cout << "UAV ID: " << id << ", Backhaul gNB Position: " << backhaulGnbPosition
                  << std::endl;

        Ptr<MobilityModel> ueMobility = ue->GetObject<MobilityModel>();
        Vector uePosition = ueMobility->GetPosition();
        std::cout << "UAV ID: " << id << ", UE Position: " << uePosition << std::endl;
    }

  private:
    Ptr<Node> fronthaulGnb;
    Ptr<Node> backhaulGnb;
    Ptr<Node> ue;
    static Ipv4Address baseNetwork;
    static uint32_t uavCount;
    uint32_t id;

    static void IncrementBaseNetwork()
    {
        uint32_t address = baseNetwork.Get();
        address += 0x01000000;
        baseNetwork.Set(address);
    }
};

void
DeployRandomUes(NodeContainer& ueContainer, uint32_t numUes)
{
    MobilityHelper mobility;

    Ptr<RandomRectanglePositionAllocator> positionAlloc =
        CreateObject<RandomRectanglePositionAllocator>();
    positionAlloc->SetAttribute("X", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=100.0]"));
    positionAlloc->SetAttribute("Y", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=100.0]"));

    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    InternetStackHelper internet;
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("20.0.0.0", "255.255.255.0");

    for (uint32_t i = 0; i < numUes; ++i)
    {
        Ptr<Node> ue = ueContainer.Get(i);
        mobility.Install(ue);
        internet.Install(ue);

        NetDeviceContainer devices = ue->GetDevice(0);
        ipv4.Assign(devices);

        uint32_t baseAddr = ipv4.NewNetwork().Get();
        baseAddr += 0x01000000;
        ipv4.SetBase(Ipv4Address(baseAddr), "255.255.255.0");
    }
}

void
DeployUavs()
{
    MobilityHelper mobility;

    // Create random number generators for x and y coordinates
    Ptr<UniformRandomVariable> xRandom = CreateObject<UniformRandomVariable>();
    xRandom->SetAttribute("Min", DoubleValue(0.0));
    xRandom->SetAttribute("Max", DoubleValue(100.0));

    Ptr<UniformRandomVariable> yRandom = CreateObject<UniformRandomVariable>();
    yRandom->SetAttribute("Min", DoubleValue(0.0));
    yRandom->SetAttribute("Max", DoubleValue(100.0));

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    for (uint32_t i = 0; i < fronthaulGnbContainer.GetN(); ++i)
    {
        Ptr<Node> fGnb = fronthaulGnbContainer.Get(i);
        Ptr<Node> bGnb = backhaulGnbContainer.Get(i);
        Ptr<Node> ueUav = ueUavContainer.Get(i);

        // Generate random x and y positions
        double x = xRandom->GetValue();
        double y = yRandom->GetValue();

        // Install mobility model
        mobility.Install(fGnb);
        mobility.Install(bGnb);
        mobility.Install(ueUav);

        // Set positions with different z coordinates
        Ptr<MobilityModel> fGnbMobility = fGnb->GetObject<MobilityModel>();
        fGnbMobility->SetPosition(
            Vector(x, y, gNbHeight)); // Example z-coordinate for fronthaul gNB

        Ptr<MobilityModel> bGnbMobility = bGnb->GetObject<MobilityModel>();
        bGnbMobility->SetPosition(
            Vector(x, y, gNbHeight + 10.0)); // Different z-coordinate for backhaul gNB

        Ptr<MobilityModel> ueUavMobility = ueUav->GetObject<MobilityModel>();
        ueUavMobility->SetPosition(Vector(x, y, ueHeight)); // Example z-coordinate for UE/UAV
    }
}

Ipv4Address UAV::baseNetwork = Ipv4Address("10.0.0.0");
uint32_t UAV::uavCount = 0;

int
main(int argc, char* argv[])
{
    CommandLine cmd;
    cmd.Parse(argc, argv);

    // Disable all logs from NetAnim
    LogComponentDisable("NetAnim", (LogLevel)0);

    UAV uav1;
    UAV uav2;
    UAV uav3;
    UAV uav4;

    DeployUavs();

    NodeContainer randomUes;
    randomUes.Create(5);
    DeployRandomUes(randomUes, randomUes.GetN());

    Ptr<NrHelper> nrHelper4f = CreateObject<NrHelper>();
    Ptr<NrHelper> nrHelper4b = CreateObject<NrHelper>();
    Ptr<IdealBeamformingHelper> idealBf = CreateObject<IdealBeamformingHelper>();
    Ptr<NrPointToPointEpcHelper> epcHelper = CreateObject<NrPointToPointEpcHelper>();

    nrHelper4b->SetBeamformingHelper(idealBf);
    nrHelper4f->SetEpcHelper(epcHelper);

    nrHelper4f->SetGnbAntennaAttribute("NumRows", UintegerValue(4));
    nrHelper4f->SetGnbAntennaAttribute("NumColumns", UintegerValue(8));
    nrHelper4f->SetGnbAntennaAttribute("AntennaElement",
                                       PointerValue(CreateObject<IsotropicAntennaModel>()));

    nrHelper4b->SetGnbAntennaAttribute("NumRows", UintegerValue(4));
    nrHelper4b->SetGnbAntennaAttribute("NumColumns", UintegerValue(8));
    nrHelper4b->SetGnbAntennaAttribute("AntennaElement",
                                       PointerValue(CreateObject<ThreeGppAntennaModel>()));

    // bwp code

    double centralFrequencyBand = 28e9;
    double bandwidthBand = 50e6;
    BandwidthPartInfoPtrVector allBwps;
    CcBwpCreator ccBwpCreator;
    const uint8_t numCcPerBand = 1;
   

    CcBwpCreator::SimpleOperationBandConf bandConf(centralFrequencyBand,
                                                   bandwidthBand,
                                                   numCcPerBand,
                                                   BandwidthPartInfo::UMi_StreetCanyon);

    OperationBandInfo band = ccBwpCreator.CreateOperationBandContiguousCc(bandConf);

    Config::SetDefault("ns3::ThreeGppChannelModel::UpdatePeriod", TimeValue(MilliSeconds(0)));
    nrHelper4f->SetChannelConditionModelAttribute("UpdatePeriod", TimeValue(MilliSeconds(0)));
    nrHelper4f->SetPathlossAttribute("ShadowingEnabled", BooleanValue(false));

    nrHelper4f->InitializeOperationBand(&band);
    nrHelper4b->InitializeOperationBand(&band);

    allBwps = CcBwpCreator::GetAllBwps({band});

    idealBf->SetAttribute("BeamformingMethod", TypeIdValue(DirectPathBeamforming::GetTypeId()));

    // Core latency
    epcHelper->SetAttribute("S1uLinkDelay", TimeValue(MilliSeconds(0)));

    NetDeviceContainer backhaulEnbNetDev =
        nrHelper4f->InstallGnbDevice(fronthaulGnbContainer, allBwps);

    NetDeviceContainer fronthaulEnbNetDev =
        nrHelper4f->InstallGnbDevice(backhaulGnbContainer, allBwps);
    NetDeviceContainer ueUavNetDev = nrHelper4f->InstallUeDevice(ueUavContainer, allBwps);

    NetDeviceContainer randomUeNetDev = nrHelper4f->InstallUeDevice(randomUes, allBwps);

    // When all the configuration is done, explicitly call UpdateConfig ()

    for (auto it = fronthaulEnbNetDev.Begin(); it != fronthaulEnbNetDev.End(); ++it)
    {
        DynamicCast<NrGnbNetDevice>(*it)->UpdateConfig();
    }

    // Set up beamforming for the cloned gNB nodes of backhaul
    for (auto it = backhaulEnbNetDev.Begin(); it != backhaulEnbNetDev.End(); ++it)
    {
        DynamicCast<NrGnbNetDevice>(*it)->UpdateConfig();
    }

    for (auto it = ueUavNetDev.Begin(); it != ueUavNetDev.End(); ++it)
    {
        DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();
    }

    for (auto it = randomUeNetDev.Begin(); it != randomUeNetDev.End(); ++it)
    {
        DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();
    }

  

    // create the internet and install the IP stack on the UEs
    // get SGW/PGW and create a single RemoteHost
    Ptr<Node> pgw = epcHelper->GetPgwNode();
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create(1);
    Ptr<Node> remoteHost = remoteHostContainer.Get(0);
    InternetStackHelper internet;
    internet.Install(remoteHostContainer);

    

    // connect a remoteHost to pgw. Setup routing too
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Gb/s")));
    p2ph.SetDeviceAttribute("Mtu", UintegerValue(2500));
    p2ph.SetChannelAttribute("Delay", TimeValue(Seconds(0.000)));
    NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);
    Ipv4AddressHelper ipv4h;
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    ipv4h.SetBase("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign(internetDevices);
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting =
        ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
    remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);

    std::cout << "---------------------------------------------\n";

    // Set the default gateway for the UEs
    for (uint32_t j = 0; j < randomUes.GetN(); ++j)
    {
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(
            randomUes.Get(j)->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }


    nrHelper4b->AttachToClosestEnb(ueUavNetDev, backhaulEnbNetDev);
    for (uint32_t i = 0; i < ueUavNetDev.GetN(); ++i)
    {
        Ptr<NrUeNetDevice> ueDevice = ueUavNetDev.Get(i)->GetObject<NrUeNetDevice>();
        Ptr<Node> ueNode = ueDevice->GetNode();
        Ptr<const NrGnbNetDevice> enbDevice = ueDevice->GetTargetEnb();
        Ptr<Node> enbNode = enbDevice->GetNode();
        std::cout << "UE " << ueNode->GetId() << " is attached to gNB (backhaul) "
                  << enbNode->GetId() << std::endl;
    }

    nrHelper4f->AttachToClosestEnb(randomUeNetDev, fronthaulEnbNetDev);

    for (uint32_t i = 0; i < randomUeNetDev.GetN(); ++i)
    {
        Ptr<NrUeNetDevice> ueDevice = randomUeNetDev.Get(i)->GetObject<NrUeNetDevice>();
        Ptr<const NrGnbNetDevice> enbDevice = ueDevice->GetTargetEnb();
        Ptr<Node> ueNode = ueDevice->GetNode();
        Ptr<Node> enbNode = enbDevice->GetNode();
        std::cout << "UE " << ueNode->GetId() << " is attached to gNB (fronthaul) "
                  << enbNode->GetId() << std::endl;
    }

    uav1.PrintPositions();
    uav2.PrintPositions();
    uav3.PrintPositions();
    uav4.PrintPositions();

    // Add UDP Echo Server and Client
    uint16_t port = 9;
    UdpEchoServerHelper echoServer(port);
    ApplicationContainer serverApps = echoServer.Install(remoteHost);
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(uav1.GetUe()->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal(),
                                   port);
    echoClient.SetAttribute("MaxPackets", UintegerValue(100));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(uav2.GetUe());
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Simulator::Stop(Seconds(10.0));

    AnimationInterface anim("ns3_animation.xml");
    anim.SetMobilityPollInterval(Seconds(1));
    anim.SetMaxPktsPerTraceFile(500000);
    anim.EnablePacketMetadata(true);

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
