# Lab3 Skeleton

from pox.core import core

import pox.openflow.libopenflow_01 as of
import pox.lib.packet as pkt
#from pox.lib.addresses import IPAddr

log = core.getLogger()

class Routing (object):
  """
  A Firewall object is created for each switch that connects.
  A Connection object for that switch is passed to the __init__ function.
  """
  # Helper Functions
  def determine_subnet(self, ip_address):
    if ip_address.startswith('200.20.2.'):
      return 'sales'
    elif ip_address.startswith('200.20.3.'):
      return 'ot'
    elif ip_address.startswith('200.20.4.'):
      return 'it'
    elif ip_address.startswith('200.20.1.'):
      return 'datacenter'
    #return None # IP is not in any subnet

  def get_output_port(self, switch, src_subnet, dst_subnet, src_ip, dst_ip):
  
    switch_to_core_port_mappings = {
      1 : 11,  # Switch1 to core switch
      2 : 2,  # Switch2 to core switch
      3 : 3,  # Switch3 to core switch
      4 : 4  # Switch4 to core switch
    }

    ip_to_port = {
        '200.20.2.8': 5,
        '200.20.2.10': 6,
        '200.20.2.9': 7,

        '200.20.1.1': 4,
        '200.20.1.2': 5,
        '200.20.1.3': 6,

        '200.20.4.6': 4,
        '200.20.4.7': 3,

        '200.20.3.4': 2,
        '200.20.3.5': 3,
    }

    if src_subnet == dst_subnet:
      # Traffic within the same subnet
      ##log.debug("subnet to subnet")
      return ip_to_port.get(dst_ip)

    else:
      # Traffic needs to go through the core switch
      if switch == 5:
        # If the packet is at the core switch, send it to the correct department switch
        ##log.debug("core to switch")
        if dst_subnet == "sales":
          return 4
        if dst_subnet == "ot":
          return 11
        if dst_subnet == "it":
          return 2
        if dst_subnet == "datacenter":
          return 3
        #return core_to_switch_port_mappings.get(dst_subnet)
      else:
        # Packet is at a department switch, send it to the core switch or deliver to dst
        # #log.debug("department switch")
        if switch == 1:
          if dst_subnet != "ot":
            return switch_to_core_port_mappings.get(switch)
          else:
            return ip_to_port.get(dst_ip)
        if switch == 2:
          if dst_subnet != "it":
            return switch_to_core_port_mappings.get(switch)
          else:
            return ip_to_port.get(dst_ip)
        if switch == 3:
          if dst_subnet != "datacenter":
            return switch_to_core_port_mappings.get(switch)
          else:
            return ip_to_port.get(dst_ip)
        if switch == 4:
          if dst_subnet != "sales":
            return switch_to_core_port_mappings.get(switch)
          else:
            return ip_to_port.get(dst_ip)

  def forward_packet(self, packet, packet_in, port_on_switch, action_port, switch_id, action):
    src_ip = str(packet.find('ipv4').srcip)
    dst_ip = str(packet.find('ipv4').dstip)
    src_subnet = self.determine_subnet(src_ip)
    dst_subnet = self.determine_subnet(dst_ip)
    msg = of.ofp_flow_mod()
    msg.match = of.ofp_match.from_packet(packet, port_on_switch)
    output_port = action_port
    msg.data = packet_in
    #log.debug("output_port type = %s", type(output_port))
    #if output_port == None:
      #self.drop_packet(packet, packet_in, port_on_switch, switch_id)
    if action == "Accept":
      msg.actions.append(of.ofp_action_output(port=output_port))
      self.connection.send(msg)
      #log.debug("packet FORWARDED from %s to %s on switch %s from port %s to port %s", src_ip, dst_ip, switch_id, port_on_switch, output_port)
    if action == "Drop":
      msg.data = None
      self.connection.send(msg)
      #log.debug("packet DROPPED from %s to %s on switch %s from port %s to port %s", src_ip, dst_ip, switch_id, port_on_switch, output_port)
  """
  def drop_packet(self, packet, packet_in, port_on_switch, switch_id):
    src_ip = str(packet.find('ipv4').srcip)
    dst_ip = str(packet.find('ipv4').dstip)
    src_subnet = self.determine_subnet(src_ip)
    dst_subnet = self.determine_subnet(dst_ip)
    msg = of.ofp_flow_mod()
    msg.match = of.ofp_match.from_packet(packet, port_on_switch)
    output_port = self.get_output_port(switch_id, src_subnet, dst_subnet, src_ip, dst_ip)
    msg.data = packet_in
    self.connection.send(msg)
    #log.debug("packet DROPPED from %s to %s on switch %s from port %s to port %s", src_ip, dst_ip, switch_id, port_on_switch, output_port)
  """
  def __init__ (self, connection):
    # Keep track of the connection to the switch so that we can
    # send it messages!
    self.connection = connection

    # This binds our PacketIn event listener
    connection.addListeners(self)
  

  def do_routing (self, packet, packet_in, port_on_switch, switch_id):
    # port_on_swtich - the port on which this packet was received
    # switch_id - the switch which received this packet

    # Your code here
    ip_packet = packet.find('ipv4')

    src_subnet = self.determine_subnet(str(ip_packet.srcip))
    dst_subnet = self.determine_subnet(str(ip_packet.dstip))
    out_port = self.get_output_port(switch_id, src_subnet, dst_subnet, str(ip_packet.srcip), str(ip_packet.dstip))

    # Implement rules based on packet type and subnets
    if ip_packet.protocol == pkt.ipv4.ICMP_PROTOCOL:
      # Rule 1: ICMP traffic rules
      if (src_subnet == 'sales' and dst_subnet == 'it') or (src_subnet == 'it' and dst_subnet == 'sales') or src_subnet == dst_subnet:
        # Forward ICMP traffic
        #out_port = self.get_output_port(switch_id, src_subnet, dst_subnet, str(ip_packet.srcip), str(ip_packet.dstip))
        self.forward_packet(packet, packet_in, port_on_switch, out_port, switch_id, "Accept")
        #log.debug("rule 1")
      else:
        # Drop the packet
        #log.debug("rule 1 Drop")
        #out_port = self.get_output_port(switch_id, src_subnet, dst_subnet, str(ip_packet.srcip), str(ip_packet.dstip))
        self.forward_packet(packet, packet_in, port_on_switch, out_port, switch_id, "Drop")
        #return self.drop_packet(packet, packet_in, port_on_switch, switch_id)
        return
    elif ip_packet.protocol == pkt.ipv4.TCP_PROTOCOL:
      # Rule 2: TCP traffic rules
      if (src_subnet in ['datacenter', 'it', 'ot'] and dst_subnet in ['datacenter', 'it','ot']) or src_subnet == dst_subnet:
        # Forward TCP traffic
        #out_port = self.get_output_port(switch_id, src_subnet, dst_subnet, str(ip_packet.srcip), str(ip_packet.dstip))
        self.forward_packet(packet, packet_in, port_on_switch, out_port, switch_id, "Accept")
        #log.debug("rule 2")
      else:
        # Drop the packet
        #out_port = self.get_output_port(switch_id, src_subnet, dst_subnet, str(ip_packet.srcip), str(ip_packet.dstip))
        self.forward_packet(packet, packet_in, port_on_switch, out_port, switch_id, "Drop")
        #log.debug("rule 2 Drop")
        #return self.drop_packet(packet, packet_in, port_on_switch, switch_id)
        return 
    elif ip_packet.protocol == pkt.ipv4.UDP_PROTOCOL:
      # Rule 3: UDP traffic rules
      if ((src_subnet in ['ot', 'it'] and dst_subnet == 'datacenter') or
              (src_subnet == 'datacenter' and dst_subnet == ['ot', 'it']) or
              src_subnet == dst_subnet):
        # Forward UDP traffic
        self.forward_packet(packet, packet_in, port_on_switch, out_port, switch_id, "Accept")
        #log.debug("rule 3")
      else:
        # Drop the packet
        self.forward_packet(packet, packet_in, port_on_switch, out_port, switch_id, "Drop")
        #log.debug("rule 3 Drop")
        #return self.drop_packet(packet, packet_in, port_on_switch, switch_id)
        return 
    else:
      # Rule 4: Drop all other traffic
      self.forward_packet(packet, packet_in, port_on_switch, out_port, switch_id, "Drop")
      #log.debug("rule 4 Drop")
      #return self.drop_packet(packet, packet_in, port_on_switch, switch_id)
      return 

  def _handle_PacketIn (self, event):
    """
    Handles packet in messages from the switch.
    """
    packet = event.parsed # This is the parsed packet data.
    if not packet.parsed:
      #log.warning("Ignoring incomplete packet")
      return

    packet_in = event.ofp # The actual ofp_packet_in message.
    self.do_routing(packet, packet_in, event.port, event.dpid)

def launch ():
  """
  Starts the component
  """
  def start_switch (event):
    #log.debug("Controlling %s" % (event.connection,))
    Routing(event.connection)
  core.openflow.addListenerByName("ConnectionUp", start_switch)



