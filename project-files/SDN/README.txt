topo.py - Mininet topology, defines all the Hosts, switches, and their coressponding IP addresses, port numbers, and links.
	- Usage: In a Linux envrionment run this command: 
		$sudo python ~/topo.py 
controller.py - POX controller file that implements the 
		network rules for the topology implemented in topo.py
	      - Rules that are implemented:
			● Rule 1: ICMP traffic is forwarded only between the Sales Department and IT
				Department subnets or between devices that are on the same subnet.
			● Rule 2: TCP traffic is forwarded only between the Datacenter, IT Department and OT
				Department subnets or between devices that are on the same subnet.
			● Rule 3: UDP traffic is forwarded only between the OT Department and Datacenter
				subnets or IT Department and Datacenter subnets or between devices that are on
				the same subnet.
			● Rule 4: All other traffic should be dropped.
	      - Usage: run this command before running topo.py:
			$sudo ~/pox.py misc.prelab3controller

opo-diagram - picture of the diagram of the topology that is implemented in topo.py






