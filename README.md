# PIOSS-LB

A Parallel I/O and Storage System (PIOSS) simulation model for the analysis of spatial aspects of load balance in parallel file systems.


## Options

`--niop=NUM | -n NUM`: number of I/O generating processes (default: 1)

`--nbytes_per_iop=NUM | -b NUM`: size (in bytes) of the contiguous block of data transferred by each I/O process (default: 1 MiB = 1048576)

`--shared_file | -f`: all I/O processes transfer data to a single shared file (default: each I/O process transfers to an independent file)

`--ndts=NUM | -m NUM`: number of data servers (default: 2)

`--stripe_width=NUM | -w NUM`: number of data servers per file; must be smaller or equal to `--ndts` (default: 1)

`--stripe_size=NUM | -z NUM`: size of a file stripe fragment (default: 64 KiB = 65536)

`--lb_type=STR | -t STR`: data server load balancer; currently two options are available: "`RAND`", and "`RAND+SEQ`" (default: RAND)

`--lb_seed=NUM | -s NUM`: seed for load balancer RNG (default: 0)

`--output=STR | -o STR`: append output statistics to the file (default: no output)

`--debug | -d`: print debug messages (default: false)

`--help | -h`: display command-line options

