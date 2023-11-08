#Create a simulator object
set ns [new Simulator]

#Define different colors for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red
$ns color 3 Pink
$ns color 4 Yellow
$ns color 5 Black

#Open the NAM trace file
set nf [open out.nam w]
$ns namtrace-all $nf

#Define a 'finish' procedure
proc finish {} {
        global ns nf
        $ns flush-trace
        #Close the NAM trace file
        close $nf
        #Execute NAM on the trace file
        exec nam out.nam &
        exit 0
}

#Create four nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

#Create links between the nodes
$ns duplex-link $n0 $n1 5840B 135ms DropTail
$ns duplex-link $n1 $n2 5840B 135ms DropTail
$ns duplex-link $n2 $n3 5840B 135ms DropTail
$ns duplex-link $n3 $n4 5840B 135ms DropTail
$ns duplex-link $n4 $n5 5840B 135ms DropTail

#Set Queue Size of link (n2-n3) to 10
$ns queue-limit $n0 $n1 50
$ns queue-limit $n1 $n2 50
$ns queue-limit $n2 $n3 50
$ns queue-limit $n3 $n4 50
$ns queue-limit $n4 $n5 50

#Give node position (for NAM)
$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n2 orient right
$ns duplex-link-op $n2 $n3 orient right
$ns duplex-link-op $n3 $n4 orient right
$ns duplex-link-op $n4 $n5 orient right

#Monitor the queue for link (n2-n3). (for NAM)
$ns duplex-link-op $n0 $n1 queuePos 0.5
$ns duplex-link-op $n1 $n2 queuePos 0.5
$ns duplex-link-op $n2 $n3 queuePos 0.5
$ns duplex-link-op $n3 $n4 queuePos 0.5
$ns duplex-link-op $n4 $n5 queuePos 0.5

#Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set class_ 2
$tcp set window_ 4.0
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n5 $sink
$ns connect $tcp $sink
$tcp set fid_ 1

#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp set packetSize_ 1
$ftp set interval _0.25
$ftp attach-agent $tcp
$ftp set type_ FTP


#Schedule events for the FTP agents
$ns at 0.1 "$ftp start"
$ns at 60.0 "$ftp stop"

#Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set class_ 2
$tcp set window_ 4.0
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink
$tcp set fid_ 2

#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp set packetSize_ 1
$ftp set interval _0.25
$ftp attach-agent $tcp
$ftp set type_ FTP


#Schedule events for the FTP agents
$ns at 0.1 "$ftp start"
$ns at 60.0 "$ftp stop"

#Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set class_ 2
$tcp set window_ 4.0
$ns attach-agent $n3 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n5 $sink
$ns connect $tcp $sink
$tcp set fid_ 3

#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp set packetSize_ 1
$ftp set interval _0.25
$ftp attach-agent $tcp
$ftp set type_ FTP


#Schedule events for the FTP agents
$ns at 0.1 "$ftp start"
$ns at 60.0 "$ftp stop"

#Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set class_ 2
$tcp set window_ 4.0
$ns attach-agent $n4 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n3 $sink
$ns connect $tcp $sink
$tcp set fid_ 4

#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp set packetSize_ 1
$ftp set interval _0.25
$ftp attach-agent $tcp
$ftp set type_ FTP


#Schedule events for the FTP agents
$ns at 0.1 "$ftp start"
$ns at 60.0 "$ftp stop"

#Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set class_ 2
$tcp set window_ 4.0
$ns attach-agent $n1 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink
$tcp set fid_ 5

#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp set packetSize_ 1
$ftp set interval _0.25
$ftp attach-agent $tcp
$ftp set type_ FTP


#Schedule events for the FTP agents
$ns at 10.1 "$ftp start"
$ns at 60.0 "$ftp stop"

#Detach tcp and sink agents (not really necessary)
$ns at 60.0 "$ns detach-agent $n0 $tcp;$ns detach-agent $n5 $sink"

#Call the finish procedure after 60 seconds of simulation time
$ns at 60.0 "finish"


#Run the simulation
$ns run