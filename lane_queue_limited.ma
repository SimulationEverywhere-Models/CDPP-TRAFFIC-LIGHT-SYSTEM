[top]
components : lane_queue@GenericQueue

in : in done stop_in stop_out
out : out next block
Link : in in@lane_queue
Link : done done@lane_queue
Link : stop_in stop_in@lane_queue
Link : stop_out stop_out@lane_queue
Link : out@lane_queue out
Link : out@lane_queue done@lane_queue
Link : block@lane_queue block
Link : next@lane_queue next

[lane_queue]
preparation : 0:0:2:000
# maximum size of the queue, 0 - unlimited
capacity :  5
input : enable
output : enable
