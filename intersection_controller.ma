[top]
components : gen@Generator queue@Queue server@IntersectionController
out : value busy timeout
Link : out@gen in@queue
Link : out@queue in@server
Link : out@queue value
Link : out@server timeout
Link : out@server done@queue
Link : busy@server busy

[gen]
distribution : normal
mean : 3
deviation : 1
initial : 1
increment : 1

[queue]
preparation : 0:0:0:100

[server]

