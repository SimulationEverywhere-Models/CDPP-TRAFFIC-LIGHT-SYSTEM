[top]
components : gen_in@Generator gen_out@Generator collector@TrafficCollector
out : throughput curlength avelength
Link : out@gen_in entered@collector
Link : out@gen_out leaved@collector
Link : throughput@collector throughput
Link : curQueueLength@collector curlength
Link : aveQueueLength@collector avelength

[gen_in]
distribution : poisson
mean : 3
#deviation : 1
initial : 1
increment : 1

[gen_out]
distribution : poisson
mean : 3
#deviation : 1
initial : 1
increment : 1

[collector]
frequence : 0:1:0:0


