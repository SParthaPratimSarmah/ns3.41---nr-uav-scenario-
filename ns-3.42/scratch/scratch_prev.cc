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
        // Assign a unique ID to this UAV
        id = ++uavCount;

        // Create the gNB and UE nodes
        fronthaulGnb = CreateObject<Node>();
        backhaulGnb = CreateObject<Node>();
        ue = CreateObject<Node>();

        // Add nodes to the global containers
        fronthaulGnbContainer.Add(fronthaulGnb);
        backhaulGnbContainer.Add(backhaulGnb);
        ueUavContainer.Add(ue);

        // Set up the P2P link helpers
        PointToPointHelper p2p;
        p2p.SetDeviceAttribute("DataRate", StringValue("10Gbps"));
        p2p.SetChannelAttribute("Delay", StringValue("2ms"));

        // Install the P2P devices and connect the nodes
        NetDeviceContainer p2pDevices;
        p2pDevices =
            p2p.Install(fronthaulGnb, backhaulGnb);   // Connect fronthaul gNB and backhaul gNB
        p2pDevices.Add(p2p.Install(backhaulGnb, ue)); // Connect backhaul gNB and UE

        // Install the internet stack on all nodes
        InternetStackHelper internet;
        internet.Install(fronthaulGnb);
        internet.Install(backhaulGnb);
        internet.Install(ue);

        // Assign IP addresses to the devices
        Ipv4AddressHelper ipv4;
        ipv4.SetBase(baseNetwork, "255.255.255.0");
        ipv4.Assign(p2pDevices);

        // Update the base network for the next UAV
        IncrementBaseNetwork();
    }

    // Accessor methods
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

    // Function to print positions of the nodes
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

    // Function to set mobility model and position for gNB and UE nodes
    void SetMobility(std::string mobilityModel)
    {
        MobilityHelper mobility;

        // Position allocator for gNB nodes
        Ptr<ListPositionAllocator> gnbPositionAlloc = CreateObject<ListPositionAllocator>();
        gnbPositionAlloc->Add(Vector(0.0, id * 4, gNbHeight)); // Position for fronthaul gNB
        gnbPositionAlloc->Add(Vector(4.0, id * 4, gNbHeight)); // Position for backhaul gNB

        // Position allocator for UE node
        Ptr<ListPositionAllocator> uePositionAlloc = CreateObject<ListPositionAllocator>();
        uePositionAlloc->Add(Vector(8.0, id * 4 + 10, ueHeight)); // Adjust position as needed

        mobility.SetMobilityModel(mobilityModel);
        mobility.SetPositionAllocator(gnbPositionAlloc);
        mobility.Install(fronthaulGnb);
        mobility.Install(backhaulGnb);

        mobility.SetPositionAllocator(uePositionAlloc);
        mobility.Install(ue);
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
        address += 0x01000000; // Increment the third octet
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
    ipv4.SetBase("11.0.0.0", "255.255.255.0");

    for (uint32_t i = 0; i < numUes; ++i)
    {
        Ptr<Node> ue = ueContainer.Get(i);
        mobility.Install(ue);

        // Install internet stack
        internet.Install(ue);

        // Assign IP addresses
        NetDeviceContainer devices = ue->GetDevice(0);
        ipv4.Assign(devices);

        // Increment the base address to avoid collisions
        uint32_t baseAddr = ipv4.NewNetwork().Get();
        baseAddr += 0x01000000; // Increment the third octet
        ipv4.SetBase(Ipv4Address(baseAddr), "255.255.255.0");
    }
}

Ipv4Address UAV::baseNetwork = "10.1.0.0";
uint32_t UAV::uavCount = 0;

int
main(int argc, char* argv[])
{
    // Create multiple UAV objects

    uint32_t numUes = 4;

    NodeContainer randomUes;
    for (uint32_t i = 0; i < numUes; ++i)
    {
        Ptr<Node> ue = CreateObject<Node>();
        randomUes.Add(ue);
    }
    DeployRandomUes(randomUes, numUes);

    UAV uav1;
    UAV uav2;
    // Set mobility model for each UAV
    std::string mobilityModel = "ns3::ConstantPositionMobilityModel";
    uav1.SetMobility(mobilityModel);
    uav2.SetMobility(mobilityModel);
    // uav3.SetMobility(mobilityModel);

    Ptr<NrHelper> nrHelper4f = CreateObject<NrHelper>();
    Ptr<NrHelper> nrHelper4b = CreateObject<NrHelper>();

    Ptr<IdealBeamformingHelper> idealBf = CreateObject<IdealBeamformingHelper>();

    // Put the pointers inside nrHelper4f
    nrHelper4b->SetBeamformingHelper(idealBf);

    // Antennas for all the UEs
    nrHelper4f->SetUeAntennaAttribute("NumRows", UintegerValue(2));
    nrHelper4f->SetUeAntennaAttribute("NumColumns", UintegerValue(4));
    nrHelper4f->SetUeAntennaAttribute("AntennaElement",
                                      PointerValue(CreateObject<ThreeGppAntennaModel>()));

    // Antennas for all the gNbs of fronthaul
    nrHelper4f->SetGnbAntennaAttribute("NumRows", UintegerValue(4));
    nrHelper4f->SetGnbAntennaAttribute("NumColumns", UintegerValue(8));
    nrHelper4f->SetGnbAntennaAttribute("AntennaElement",
                                       PointerValue(CreateObject<IsotropicAntennaModel>()));

    // Antennas for all the gNbs of backhaul
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

    std::cout << "---------------------------------------------\n";
    nrHelper4b->AttachToClosestEnb(ueUavNetDev, backhaulEnbNetDev);

    nrHelper4f->AttachToClosestEnb(randomUeNetDev, fronthaulEnbNetDev);

    uint16_t dlPort = 1234;
    uint16_t ulPort = 2000;

    ApplicationContainer clientApps, serverApps;

    // Install a UDP server on the UEs
    for (NodeContainer::Iterator it = randomUes.Begin(); it != randomUes.End(); ++it)
    {
        UdpServerHelper dlServer(dlPort);
        serverApps.Add(dlServer.Install(*it));
        UdpServerHelper ulServer(ulPort);
        serverApps.Add(ulServer.Install(*it));
    }

    // Install a UDP client on the gNBs
    for (NodeContainer::Iterator it = fronthaulGnbContainer.Begin();
         it != fronthaulGnbContainer.End();
         ++it)
    {
        UdpClientHelper dlClient(Ipv4Address("7.0.0.2"),
                                 dlPort); // Use the actual IP address of UEs
        dlClient.SetAttribute("MaxPackets", UintegerValue(320));
        dlClient.SetAttribute("Interval", TimeValue(MilliSeconds(10)));
        dlClient.SetAttribute("PacketSize", UintegerValue(1024));
        clientApps.Add(dlClient.Install(*it));

        UdpClientHelper ulClient(Ipv4Address("7.0.0.1"),
                                 ulPort); // Use the actual IP address of gNBs
        ulClient.SetAttribute("MaxPackets", UintegerValue(320));
        ulClient.SetAttribute("Interval", TimeValue(MilliSeconds(10)));
        ulClient.SetAttribute("PacketSize", UintegerValue(1024));
        clientApps.Add(ulClient.Install(*it));
    }

    serverApps.Start(Seconds(0.1));
    clientApps.Start(Seconds(0.1));
    serverApps.Stop(Seconds(.8));
    clientApps.Stop(Seconds(.8));

    // Print positions of each UAV's nodes
    uav1.PrintPositions();
    uav2.PrintPositions();
    // uav3.PrintPositions();

    // Enable logging only for critical components
    LogComponentEnable("NrHelper", LOG_LEVEL_WARN);
    LogComponentEnable("NrGnbNetDevice", LOG_LEVEL_WARN);

    // Enable the animation interface
    AnimationInterface anim("animation.xml"); // Output file name

    Simulator::Stop();
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
