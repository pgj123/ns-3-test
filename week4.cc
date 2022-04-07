#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Week4Ex1");

int
main(int argc, char *argv[]){
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoClientApplication", LOG_PREFIX_TIME);
    LogComponentEnable("UdpEchoClientApplication", LOG_PREFIX_FUNC);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_PREFIX_TIME);
    LogComponentEnable("UdpEchoServerApplication", LOG_PREFIX_FUNC);

    /* nodeContainer ~ */
    NodeContainer nodes;
    nodes.Create(3);

    NodeContainer n0n1 = NodeContainer(nodes.Get(0), nodes.Get(1));
    NodeContainer n1n2 = NodeContainer(nodes.Get(1), nodes.Get(2));
    /* nodeContainer ~ */

    /* p2p ~   */
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("5ms"));

    PointToPointHelper pointToPoint2;
    pointToPoint2.SetDeviceAttribute("DataRate", StringValue("0.1Mbps"));
    pointToPoint2.SetChannelAttribute("Delay", StringValue("10ms"));
    /* p2p ~   */

    /* netDevice ~ */
    NetDeviceContainer devices;
    devices = pointToPoint.Install(n0n1);

    NetDeviceContainer devices2;
    devices2 = pointToPoint2.Install(n1n2);
    /* netDevice ~ */

    /* internetStackhelper ~ */
    InternetStackHelper stack;
    stack.Install(nodes);

    /* internetStackhelper ~ */

    /* ipv4addresshelper ~ */
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    Ipv4AddressHelper address2;
    address2.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces2 = address2.Assign(devices2);
    /* ipv4addresshelper ~ */

    /* udpechoClientHelper ~ */
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1500));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(0.001)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1050));

    UdpEchoClientHelper echoClient2(interfaces2.GetAddress(1), 9);
    echoClient2.SetAttribute("MaxPackets", UintegerValue(1500));
    echoClient2.SetAttribute("Interval", TimeValue(Seconds(0.01)));
    echoClient2.SetAttribute("PacketSize", UintegerValue(1050));
    /* udpechoClientHelper ~ */

    /* ApplicationContainer ~*/
    ApplicationContainer clientApps;
    clientApps.Add(echoClient.Install(n0n1.Get(0)));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(4.0));

    ApplicationContainer clientApps2;
    clientApps2.Add(echoClient2.Install(n1n2.Get(0)));
    clientApps2.Start(Seconds(2.0));
    clientApps2.Stop(Seconds(4.0));
    /* ApplicationContainer ~*/

    /* udpechoServerHelper ~ */
    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps(echoServer.Install(n0n1.Get(1)));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(5.0));

    UdpEchoServerHelper echoServer2(9);
    ApplicationContainer serverApps2(echoServer2.Install(n1n2.Get(1)));
    serverApps2.Start(Seconds(1.0));
    serverApps2.Stop(Seconds(5.0));
    /* udpechoServerHelper ~ */

    Simulator::Run();
    Simulator::Stop(Seconds(5.0));

    Simulator::Destroy();
    return 0;
}
