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
$ns attach-agent $n1 $sink
$ns connect $tcp $sink

set data_source [new Application/Traffic/Exponential]
$data_source set packet_size_ 1460
$data_source set burst_time_ 0.125
$data_source set idle_time_ 0.125
$data_source attach-agent $tcp
$data_source set rate_ 1mb

set tcp1 [new Agent/TCP]
$tcp1 set class_ 1
$ns attach-agent $n1 $tcp1
set sink1 [new Agent/TCPSink]
$ns attach-agent $n2 $sink1
$ns connect $tcp1 $sink1

set data_source1 [new Application/Traffic/Exponential]
$data_source1 set packet_size_ 1460
$data_source1 set burst_time_ 0.125
$data_source1 set idle_time_ 0.125
$data_source1 attach-agent $tcp1
$data_source1 set rate_ 1mb

set tcp2 [new Agent/TCP]
$tcp2 set class_ 3
$ns attach-agent $n2 $tcp2
set sink2 [new Agent/TCPSink]
$ns attach-agent $n3 $sink2
$ns connect $tcp2 $sink2

set data_source2 [new Application/Traffic/Exponential]
$data_source2 set packet_size_ 1460
$data_source2 set burst_time_ 0.125
$data_source2 set idle_time_ 0.125
$data_source2 attach-agent $tcp2
$data_source2 set rate_ 1mb

set tcp3 [new Agent/TCP]
$tcp3 set class_ 4
$ns attach-agent $n3 $tcp3
set sink3 [new Agent/TCPSink]
$ns attach-agent $n4 $sink3
$ns connect $tcp3 $sink3

set data_source3 [new Application/Traffic/Exponential]
$data_source3 set packet_size_ 1460
$data_source3 set burst_time_ 0.125
$data_source3 set idle_time_ 0.125
$data_source3 attach-agent $tcp3
$data_source3 set rate_ 1mb

set window_sizes {4 8 32}


	$tcp set window_ 4
	$ns at 0.1 "$data_source start"
	$ns at 10.0 "$data_source stop"
	$tcp1 set window_ 8
	$ns at 1.0 "$data_source1 start"
	$ns at 45.0 "$data_source1 stop"
	$tcp2 set window_ 8
	$ns at 2.1 "$data_source3 start"
	$ns at 39.0 "$data_source3 stop"
	$tcp3 set window_ 32
	$ns at 2.0 "$data_source2 start"
	$ns at 40.0 "$data_source2 stop"
	

$ns at 60.0 "finish"
#Run the simulation
$ns run
