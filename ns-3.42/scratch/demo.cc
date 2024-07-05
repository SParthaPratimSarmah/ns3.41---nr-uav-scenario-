#include "ns3/antenna-module.h"
#include "ns3/applications-module.h"
#include "ns3/buildings-module.h"
#include "ns3/config-store-module.h"
#include "ns3/core-module.h"
#include "ns3/epc-x2.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/network-module.h"
#include "ns3/nr-module.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("CttcNrDemo");

Ptr<Node>
CopyNode(Ptr<Node> originalNode)
{
    Ptr<Node> newNode = CreateObject<Node>();

    // Copy mobility model (position)
    Ptr<MobilityModel> mobOriginal = originalNode->GetObject<MobilityModel>();
    Ptr<ConstantPositionMobilityModel> mobDuplicate = CreateObject<ConstantPositionMobilityModel>();

    // Adjust position: Shift 1 unit in x and 1 unit in y
    Vector originalPosition = mobOriginal->GetPosition();
    Vector newPosition =
        originalPosition + Vector(5.0, 3.0, 0.0); // Shift by 5 units in x and 3 units in y

    mobDuplicate->SetPosition(newPosition);
    newNode->AggregateObject(mobDuplicate);

    return newNode;
}

int
main(int argc, char* argv[])
{
    uint16_t gNbNum = 2;
    uint16_t ueNumPergNb = 2;
    bool logging = true;
    uint32_t udpPacketSizeULL = 100;
    uint32_t udpPacketSizeBe = 1252;
    uint32_t lambdaULL = 10000;
    uint32_t lambdaBe = 10000;
    Time simTime = MilliSeconds(1000);
    Time udpAppStartTime = MilliSeconds(400);
    uint16_t numerologyBwp = 4;
    double centralFrequencyBand = 28e9;
    double bandwidthBand = 50e6;
    double totalTxPower = 35;

    // Where we will store the output files.
    std::string simTag = "default";
    std::string outputDir = "./";

    CommandLine cmd(__FILE__);
    cmd.AddValue("gNbNum", "The number of gNbs in multiple-ue topology", gNbNum);
    cmd.AddValue("ueNumPergNb", "The number of UE per gNb in multiple-ue topology", ueNumPergNb);
    cmd.AddValue("logging", "Enable logging", logging);
    cmd.AddValue("packetSizeUll",
                 "packet size in bytes to be used by ultra low latency traffic",
                 udpPacketSizeULL);
    cmd.AddValue("packetSizeBe",
                 "packet size in bytes to be used by best effort traffic",
                 udpPacketSizeBe);
    cmd.AddValue("lambdaUll",
                 "Number of UDP packets in one second for ultra low latency traffic",
                 lambdaULL);
    cmd.AddValue("lambdaBe",
                 "Number of UDP packets in one second for best effort traffic",
                 lambdaBe);
    cmd.AddValue("simTime", "Simulation time", simTime);
    cmd.AddValue("numerologyBwp", "The numerology to be used in bandwidth part 1", numerologyBwp);
    cmd.AddValue("centralFrequencyBand",
                 "The system frequency to be used in band 1",
                 centralFrequencyBand);
    cmd.AddValue("bandwidthBand", "The system bandwidth to be used in band 1", bandwidthBand);
    cmd.AddValue("totalTxPower",
                 "total tx power that will be proportionally assigned to"
                 " bands, CCs and bandwidth parts depending on each BWP bandwidth ",
                 totalTxPower);
    cmd.AddValue("simTag",
                 "tag to be appended to output filenames to distinguish simulation campaigns",
                 simTag);
    cmd.AddValue("outputDir", "directory where to store simulation results", outputDir);
    cmd.Parse(argc, argv);
    NS_ABORT_IF(centralFrequencyBand < 0.5e9 && centralFrequencyBand > 100e9);

    Config::SetDefault("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue(999999999));
    int64_t randomStream = 1;

    // Create gNB nodes
    NodeContainer fronthaulGnb;
    NodeContainer backhaulGnb;
    for (uint32_t i = 0; i < gNbNum; ++i)
    {
        Ptr<Node> gnb = CreateObject<Node>();
        fronthaulGnb.Add(gnb);
    }

    // Manually set positions of gNBs
    MobilityHelper gnbMobility;
    gnbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    gnbMobility.Install(fronthaulGnb);

    // gnbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    // gnbMobility.Install(backhaulGnb);

    Ptr<ConstantPositionMobilityModel> gnbMobilityModel;
    for (uint32_t i = 0; i < gNbNum; ++i)
    {
        gnbMobilityModel = fronthaulGnb.Get(i)->GetObject<ConstantPositionMobilityModel>();
        Vector gnbPosition(i * 20.0, 0.0, 10.0); // Set positions as needed (e.g., 20m apart)
        gnbMobilityModel->SetPosition(gnbPosition);
    }

    // Clone gNB nodes

    for (uint32_t i = 0; i < gNbNum; ++i)
    {
        Ptr<Node> originalGnb = fronthaulGnb.Get(i);
        Ptr<Node> clonedGnb = CopyNode(originalGnb);
        backhaulGnb.Add(clonedGnb);
    }

    // Create UE nodes and set their positions
    NodeContainer ueContainer;
    for (uint32_t i = 0; i < ueNumPergNb * gNbNum; ++i)
    {
        Ptr<Node> ue = CreateObject<Node>();
        ueContainer.Add(ue);
    }

    // setting mobility model of ue
    MobilityHelper ueMobility;
    ueMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    ueMobility.Install(ueContainer);

    Ptr<ConstantPositionMobilityModel> ueMobilityModel;
    for (uint32_t i = 0; i < ueContainer.GetN(); ++i)
    {
        ueMobilityModel = ueContainer.Get(i)->GetObject<ConstantPositionMobilityModel>();
        Vector uePosition(i * 5.0, 10.0, 1.5); // Set positions as needed (e.g., 5m apart)
        ueMobilityModel->SetPosition(uePosition);
    }

    // Print positions of UEs
    for (uint32_t j = 0; j < ueContainer.GetN(); ++j)
    {
        Ptr<Node> ue = ueContainer.Get(j);
        Ptr<MobilityModel> mobility = ue->GetObject<MobilityModel>();
        NS_LOG_UNCOND("UE " << j << " Position: " << mobility->GetPosition());
    }

    // Print positions of gNBs
    for (uint32_t i = 0; i < fronthaulGnb.GetN(); ++i)
    {
        Ptr<Node> gnb = fronthaulGnb.Get(i);
        Ptr<MobilityModel> mobility = gnb->GetObject<MobilityModel>();
        NS_LOG_UNCOND("gNB " << i << " Position: " << mobility->GetPosition());
    }

    // Create point-to-point link between original and cloned gNB
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Mbps")));
    p2p.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));

    NetDeviceContainer p2pDevices;
    for (uint32_t i = 0; i < gNbNum; ++i)
    {
        p2pDevices = p2p.Install(fronthaulGnb.Get(i), backhaulGnb.Get(i));
    }

    /*
     * TODO: Add a print, or a plot, that shows the scenario.
     */
    // NS_LOG_INFO("Creating " << gridScenario.GetUserTerminals().GetN() << " user terminals and "
    //                         << gridScenario.GetBaseStations().GetN() << " gNBs");

    Ptr<NrPointToPointEpcHelper> epcHelper = CreateObject<NrPointToPointEpcHelper>();
    Ptr<IdealBeamformingHelper> idealBeamformingHelper = CreateObject<IdealBeamformingHelper>();
    Ptr<NrHelper> nrHelper4f = CreateObject<NrHelper>();
    Ptr<NrHelper> nrHelper4b = CreateObject<NrHelper>();

    // Put the pointers inside nrHelper4f
    nrHelper4b->SetBeamformingHelper(idealBeamformingHelper);

    nrHelper4f->SetEpcHelper(epcHelper);

    BandwidthPartInfoPtrVector allBwps;
    CcBwpCreator ccBwpCreator;
    const uint8_t numCcPerBand = 1;

    CcBwpCreator::SimpleOperationBandConf bandConf1(centralFrequencyBand,
                                                    bandwidthBand,
                                                    numCcPerBand,
                                                    BandwidthPartInfo::UMi_StreetCanyon);

    // By using the configuration created, it is time to make the operation bands
    OperationBandInfo band1 = ccBwpCreator.CreateOperationBandContiguousCc(bandConf1);

    Config::SetDefault("ns3::ThreeGppChannelModel::UpdatePeriod", TimeValue(MilliSeconds(0)));
    nrHelper4f->SetChannelConditionModelAttribute("UpdatePeriod", TimeValue(MilliSeconds(0)));
    nrHelper4f->SetPathlossAttribute("ShadowingEnabled", BooleanValue(false));

    nrHelper4f->InitializeOperationBand(&band1);

    double x = pow(10, totalTxPower / 10);
    double totalBandwidth = bandwidthBand;

    allBwps = CcBwpCreator::GetAllBwps({band1});

    Packet::EnableChecking();
    Packet::EnablePrinting();

    idealBeamformingHelper->SetAttribute("BeamformingMethod",
                                         TypeIdValue(DirectPathBeamforming::GetTypeId()));

    Ptr<IdealBeamformingHelper> clonedBeamformingHelper = CreateObject<IdealBeamformingHelper>();
    clonedBeamformingHelper->SetAttribute("BeamformingMethod",
                                          TypeIdValue(DirectPathBeamforming::GetTypeId()));

    // Core latency
    epcHelper->SetAttribute("S1uLinkDelay", TimeValue(MilliSeconds(0)));

    // Antennas for all the UEs
    nrHelper4f->SetUeAntennaAttribute("NumRows", UintegerValue(2));
    nrHelper4f->SetUeAntennaAttribute("NumColumns", UintegerValue(4));
    nrHelper4f->SetUeAntennaAttribute("AntennaElement",
                                      PointerValue(CreateObject<IsotropicAntennaModel>()));

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

    uint32_t bwpIdFor = 0;

    // gNb routing between Bearer and bandwidh part
    nrHelper4f->SetGnbBwpManagerAlgorithmAttribute("NGBR_LOW_LAT_EMBB", UintegerValue(bwpIdFor));

    // Ue routing between Bearer and bandwidth part
    nrHelper4f->SetUeBwpManagerAlgorithmAttribute("NGBR_LOW_LAT_EMBB", UintegerValue(bwpIdFor));

    NetDeviceContainer enbNetDev4f = nrHelper4f->InstallGnbDevice(fronthaulGnb, allBwps);
    NetDeviceContainer enbNetDev4b = nrHelper4f->InstallGnbDevice(backhaulGnb, allBwps);
    NetDeviceContainer ueNetDev = nrHelper4f->InstallUeDevice(ueContainer, allBwps);

    randomStream += nrHelper4f->AssignStreams(enbNetDev4f, randomStream);
    randomStream += nrHelper4f->AssignStreams(ueNetDev, randomStream);

    nrHelper4f->GetGnbPhy(enbNetDev4f.Get(0), 0)
        ->SetAttribute("Numerology", UintegerValue(numerologyBwp));
    nrHelper4f->GetGnbPhy(enbNetDev4f.Get(0), 0)
        ->SetAttribute("TxPower", DoubleValue(10 * log10((bandwidthBand / totalBandwidth) * x)));

    // When all the configuration is done, explicitly call UpdateConfig ()

    for (auto it = enbNetDev4f.Begin(); it != enbNetDev4f.End(); ++it)
    {
        DynamicCast<NrGnbNetDevice>(*it)->UpdateConfig();
    }

    // Set up beamforming for the cloned gNB nodes of backhaul
    for (auto it = enbNetDev4b.Begin(); it != enbNetDev4b.End(); ++it)
    {
        DynamicCast<NrGnbNetDevice>(*it)->UpdateConfig();
    }

    for (auto it = ueNetDev.Begin(); it != ueNetDev.End(); ++it)
    {
        DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();
    }

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
    internet.Install(ueContainer);

    Ipv4InterfaceContainer ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueNetDev));

    for (uint32_t j = 0; j < ueContainer.GetN(); ++j)
    {
        // Print UE IP address
        Ipv4Address ueIp = ueIpIface.GetAddress(j);
        NS_LOG_INFO("UE " << j << " IP Address: " << ueIp);
    }

    for (uint32_t j = 0; j < ueContainer.GetN(); ++j)
    {
        Ptr<Ipv4StaticRouting> ueStaticRouting =
            ipv4RoutingHelper.GetStaticRouting(ueContainer.Get(j)->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    // attach UEs to the closest eNB
    nrHelper4f->AttachToClosestEnb(ueNetDev, enbNetDev4f);

    // nrHelper4b->AttachEnbToEnb(ueNetDev, enbNetDev4f);

    uint16_t dlport = 1234;

    ApplicationContainer serverApps;

    // The sink will always listen to the specified ports
    UdpServerHelper dlPacketSink(dlport);

    // The server, that is the application which is listening, is installed in the UE
    serverApps.Add(dlPacketSink.Install(ueContainer));

    UdpClientHelper dlClient;
    dlClient.SetAttribute("RemotePort", UintegerValue(dlport));
    dlClient.SetAttribute("MaxPackets", UintegerValue(0xFFFFFFFF));
    dlClient.SetAttribute("PacketSize", UintegerValue(udpPacketSizeULL));
    dlClient.SetAttribute("Interval", TimeValue(Seconds(1.0 / lambdaULL)));

    // Print UE IP addresses
    for (uint32_t j = 0; j < ueContainer.GetN(); ++j)
    {
        Ipv4Address ueIp = ueIpIface.GetAddress(j);
        NS_LOG_UNCOND("UE " << j << " IP Address: " << ueIp);
    }
    ApplicationContainer clientApps;

    for (uint32_t i = 0; i < ueContainer.GetN(); ++i)
    {
        Ptr<Node> ue = ueContainer.Get(i);
        Ptr<NetDevice> ueDevice = ueNetDev.Get(i);
        Address ueAddress = ueIpIface.GetAddress(i);

        dlClient.SetAttribute("RemoteAddress", AddressValue(ueAddress));
        clientApps.Add(dlClient.Install(remoteHost));
    }

    // start UDP server and client apps
    serverApps.Start(udpAppStartTime);
    clientApps.Start(udpAppStartTime);
    serverApps.Stop(simTime);
    clientApps.Stop(simTime);

    // enable the traces provided by the nr module
    // nrHelper4f->EnableTraces();

    FlowMonitorHelper flowmonHelper;
    NodeContainer endpointNodes;
    endpointNodes.Add(remoteHost);
    endpointNodes.Add(ueContainer);

    Ptr<ns3::FlowMonitor> monitor = flowmonHelper.Install(endpointNodes);
    monitor->SetAttribute("DelayBinWidth", DoubleValue(0.001));
    monitor->SetAttribute("JitterBinWidth", DoubleValue(0.001));
    monitor->SetAttribute("PacketSizeBinWidth", DoubleValue(20));

    // create the animation
    AnimationInterface anim("animation.xml");

    Simulator::Stop(simTime);
    Simulator::Run();

    // Print per-flow statistics
    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier =
        DynamicCast<Ipv4FlowClassifier>(flowmonHelper.GetClassifier());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();

    double averageFlowThroughput = 0.0;
    double averageFlowDelay = 0.0;

    std::ofstream outFile;
    std::string filename = outputDir + "/" + simTag;
    outFile.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
    if (!outFile.is_open())
    {
        std::cerr << "Can't open file " << filename << std::endl;
        return 1;
    }

    outFile.setf(std::ios_base::fixed);

    double flowDuration = (simTime - udpAppStartTime).GetSeconds();
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin();
         i != stats.end();
         ++i)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
        std::stringstream protoStream;
        protoStream << (uint16_t)t.protocol;
        if (t.protocol == 6)
        {
            protoStream.str("TCP");
        }
        if (t.protocol == 17)
        {
            protoStream.str("UDP");
        }
        outFile << "Flow " << i->first << " (" << t.sourceAddress << ":" << t.sourcePort << " -> "
                << t.destinationAddress << ":" << t.destinationPort << ") proto "
                << protoStream.str() << "\n";
        outFile << "  Tx Packets: " << i->second.txPackets << "\n";
        outFile << "  Tx Bytes:   " << i->second.txBytes << "\n";
        outFile << "  TxOffered:  " << i->second.txBytes * 8.0 / flowDuration / 1000.0 / 1000.0
                << " Mbps\n";
        outFile << "  Rx Bytes:   " << i->second.rxBytes << "\n";
        if (i->second.rxPackets > 0)
        {
            // Measure the duration of the flow from receiver's perspective
            averageFlowThroughput += i->second.rxBytes * 8.0 / flowDuration / 1000 / 1000;
            averageFlowDelay += 1000 * i->second.delaySum.GetSeconds() / i->second.rxPackets;

            outFile << "  Throughput: " << i->second.rxBytes * 8.0 / flowDuration / 1000 / 1000
                    << " Mbps\n";
            outFile << "  Mean delay:  "
                    << 1000 * i->second.delaySum.GetSeconds() / i->second.rxPackets << " ms\n";
            // outFile << "  Mean upt:  " << i->second.uptSum / i->second.rxPackets / 1000/1000 << "
            // Mbps \n";
            outFile << "  Mean jitter:  "
                    << 1000 * i->second.jitterSum.GetSeconds() / i->second.rxPackets << " ms\n";
        }
        else
        {
            outFile << "  Throughput:  0 Mbps\n";
            outFile << "  Mean delay:  0 ms\n";
            outFile << "  Mean jitter: 0 ms\n";
        }
        outFile << "  Rx Packets: " << i->second.rxPackets << "\n";
    }

    double meanFlowThroughput = averageFlowThroughput / stats.size();
    double meanFlowDelay = averageFlowDelay / stats.size();

    outFile << "\n\n  Mean flow throughput: " << meanFlowThroughput << "\n";
    outFile << "  Mean flow delay: " << meanFlowDelay << "\n";

    outFile.close();

    std::ifstream f(filename.c_str());

    if (f.is_open())
    {
        std::cout << f.rdbuf();
    }

    Simulator::Destroy();
}
