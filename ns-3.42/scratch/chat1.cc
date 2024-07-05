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
    ipv4.SetBase("12.0.0.0", "255.255.255.0");

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

    Ptr<RandomRectanglePositionAllocator> positionAlloc =
        CreateObject<RandomRectanglePositionAllocator>();
    positionAlloc->SetAttribute("X", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=100.0]"));
    positionAlloc->SetAttribute("Y", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=100.0]"));

    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    for (uint32_t i = 0; i < fronthaulGnbContainer.GetN(); ++i)
    {
        Ptr<Node> fGnb = fronthaulGnbContainer.Get(i);
        Ptr<Node> bGnb = backhaulGnbContainer.Get(i);
        Ptr<Node> ueUav = ueUavContainer.Get(i);

        mobility.Install(fGnb);
        mobility.Install(bGnb);
        mobility.Install(ueUav);
    }
}

Ipv4Address UAV::baseNetwork = Ipv4Address("10.0.0.0");
uint32_t UAV::uavCount = 0;

int
main(int argc, char* argv[])
{
    CommandLine cmd;
    cmd.Parse(argc, argv);

    UAV uav1;
    UAV uav2;

    DeployUavs();

    NodeContainer randomUes;
    randomUes.Create(5);
    DeployRandomUes(randomUes, randomUes.GetN());

    Ptr<NrHelper> nrHelper4f = CreateObject<NrHelper>();
    Ptr<NrHelper> nrHelper4b = CreateObject<NrHelper>();
    Ptr<IdealBeamformingHelper> idealBf = CreateObject<IdealBeamformingHelper>();

    nrHelper4b->SetBeamformingHelper(idealBf);

    nrHelper4f->SetGnbAntennaAttribute("NumRows", UintegerValue(4));
    nrHelper4f->SetGnbAntennaAttribute("NumColumns", UintegerValue(8));
    nrHelper4f->SetGnbAntennaAttribute("AntennaElement",
                                       PointerValue(CreateObject<IsotropicAntennaModel>()));

    nrHelper4b->SetGnbAntennaAttribute("NumRows", UintegerValue(4));
    nrHelper4b->SetGnbAntennaAttribute("NumColumns", UintegerValue(8));
    nrHelper4b->SetGnbAntennaAttribute("AntennaElement",
                                       PointerValue(CreateObject<ThreeGppAntennaModel>()));

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

    Config::SetDefault("ns3::ThreeGppChannelModel::UpdatePeriod", TimeValue(MilliSeconds(100)));
    Config::SetDefault("ns3::Nr3gppPropagationLossModel::Frequency",
                       DoubleValue(centralFrequencyBand));
    Config::SetDefault("ns3::Nr3gppPropagationLossModel::Shadowing", BooleanValue(false));
    Config::SetDefault("ns3::Nr3gppPropagationLossModel::Blockage", BooleanValue(false));

    Ptr<ChannelConditionModel> buildCond = CreateObject<BuildingsChannelConditionModel>();
    Ptr<ChannelConditionModel> discCond = CreateObject<BuildingsChannelConditionModel>();

    // nrHelper4f->SetChannelConditionModel(buildCond);
    // nrHelper4b->SetChannelConditionModel(discCond);

    allBwps = CcBwpCreator::GetAllBwps({band});

    NetDeviceContainer gnbDevices;
    NetDeviceContainer ueDevices;

    gnbDevices.Add(nrHelper4f->InstallGnbDevice(fronthaulGnbContainer, allBwps));
    gnbDevices.Add(nrHelper4b->InstallGnbDevice(backhaulGnbContainer, allBwps));

    // ueDevices = nrHelper4f->InstallUeDevice(ueUavContainer);
    // ueDevices.Add(nrHelper4b->InstallUeDevice(randomUes));

   
        nrHelper4f->AttachToClosestEnb(ueDevices,gnbDevices);
    

    Simulator::Stop(Seconds(1.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
