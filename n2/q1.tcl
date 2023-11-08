#Create a simulator object
set ns [new Simulator]

#Define different colors for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red
$ns color 3 Green
$ns color 4 Purple

#Open the NAM trace file
set nf [open out2.nam w]
$ns namtrace-all $nf

#Define a 'finish' procedure
proc finish {} {
        global ns nf
        $ns flush-trace
        #Close the NAM trace file
        close $nf
        #Execute NAM on the trace file
        exec nam out2.nam &
        exit 0
}
#Create four nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]




$ns duplex-link $n0 $n1 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 2Mb 10ms DropTail
$ns duplex-link $n3 $n4 2Mb 10ms DropTail



$ns queue-limit $n0 $n1 50
$ns queue-limit $n1 $n2 50
$ns queue-limit $n2 $n3 50
$ns queue-limit $n3 $n4 50


$ns duplex-link-op $n0 $n1 orient center
$ns duplex-link-op $n1 $n2 orient center-left
$ns duplex-link-op $n2 $n3 orient center-left
$ns duplex-link-op $n3 $n4 orient center-left


$ns duplex-link-op $n0 $n1 queuePos 0.5
$ns duplex-link-op $n1 $n2 queuePos 0.5
$ns duplex-link-op $n2 $n3 queuePos 0.5
$ns duplex-link-op $n3 $n4 queuePos 0.5





set tcp [new Agent/TCP]
$tcp set class_ 2
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n4 $sink
$ns connect $tcp $sink

set data_source [new Application/Traffic/Exponential]
$data_source set packet_size_ 1460
$data_source set burst_time_ 0.125
$data_source set idle_time_ 0.125
$data_source attach-agent $tcp
$data_source set rate_ 1mb


set window_sizes {4 8 32}


	$tcp set window_ 4
	$ns at 0.1 "$data_source start"
	$ns at 10.0 "$data_source stop"
	$tcp set window_ 8
	$ns at 11.0 "$data_source start"
	$ns at 25.0 "$data_source stop"
	
	$tcp set window_ 32
	$ns at 26.0 "$data_source start"
	$ns at 40.0 "$data_source stop"
	

$ns at 60.0 "finish"
#Run the simulation
$ns run
