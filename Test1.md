# Description:
#       Models a (low budget) data center that could theoretically be used by
#       an upcoming streaming service startup. Goal of data center is to serve
#       streaming requests quickly while making a quick buck on the side to
#       have a less red cash flow.
#
# Machines:
#       Various classes of machines, with low performance machines for off-hours
#       and high performance for peak hours.
#
# Tasks:
#       Tasks are modelled as mainly streaming services at peak hours and other side
#       tasks during off hours. Streaming service related tasks will have a
#       higher SLA requirement and are expected to be prioritized because of
#       this over the "side tasks" which the data center takes on to make
#       some use of the machines during off hours (perhaps the data center wants
#       to do some AI modeling on what shows will be watched more the
#       next day or sell off some computing power).
#
# Notes:
#       Not sure what are reasonable energy usages for S, P, and C states. Adjusted
#       from sample to represent the performance difference between the machine
#       classes.



# Rented out high performance machines with many cores to increase throughput.
# Speed makes up for the high power usage. Memory used for each core is
# likely not large since the network is the bottleneck. However it is
# possible that frequently accessed content are cached to reduce disk usage.
# Giving a fair 1 GB of memory per core.

machine class:
{
        Number of machines: 2
        CPU type: X86
        Number of cores: 32
        Memory: 32768
        S-States: [180, 120, 120, 90, 50, 15, 0]
        P-States: [20, 15, 10, 5]
        C-States: [12, 4, 2, 0]
        MIPS: [4000, 2800, 1800, 600]
        GPUs: no
}

# Loaned to them by a friend
machine class:
{
        Number of machines: 1
        CPU type: POWER
        Number of cores: 16
        Memory: 8192
        S-States: [120, 100, 100, 80, 40, 10, 0]
        P-States: [12, 8, 6, 4]
        C-States: [12, 3, 1, 0]
        MIPS: [1600, 1200, 800, 400]
        GPUs: yes
}


# General machines with few cores. Apple M1 MacBook Air Laptops bought in at
# sharp discount from people upgrading to M4. To simulate the 4 performance and
# 4 efficiency cores, two separate machines are used.
# Spec for 'laptop' in whole (will be split across the cores)
#       - 16 GB memory, split so 10 GB goes to performance, 6 to efficiency
#       - 200 Watt max power usage (according to online sources its 50, but
#       since we 'split' it across to two machines, 100 is easier to play with)
#       - 3.2 GHz for performance, 2 GHz for efficiency

# Performance core
machine class:
{
        Number of machines: 8
        Number of cores: 4
        CPU type: ARM
        Memory: 10240
        S-States: [20, 15, 15, 12, 6, 2, 0]
        P-States: [16, 12, 6, 4]
        C-States: [12, 3, 1, 0]
        MIPS: [3200, 2000, 1200, 600]
        GPUs: yes
}

# Efficiency core
machine class:
{
        Number of machines: 8
        Number of cores: 4
        CPU type: ARM
        Memory: 6144
        S-States: [12, 8, 8, 6, 2, 1, 0]
        P-States: [8, 6, 4, 2]
        C-States: [6, 3, 1, 0]
        MIPS: [2000, 1200, 800, 400]
        GPUs: no
}

# Tasks
# Streaming Daily load spike
task class:
{
        Start time: 1000000
        End time : 6000000
        Inter arrival: 400
        Expected runtime: 40000
        Memory: 32
        VM type: LINUX
        GPU enabled: no
        SLA type: SLA0
        CPU type: ARM
        Task type: STREAM
        Seed: 0
}

# Average Streaming through out day
task class:
{
        Start time: 0
        End time : 10000000
        Inter arrival: 2400
        Expected runtime: 120000
        Memory: 256
        VM type: LINUX
        GPU enabled: no
        SLA type: SLA0
        CPU type: ARM
        Task type: STREAM
        Seed: 1
}

task class:
{
        Start time: 500000
        End time : 10000000
        Inter arrival: 1000000
        Expected runtime: 20000000
        Memory: 2048
        VM type: LINUX
        GPU enabled: no
        SLA type: SLA0
        CPU type: ARM
        Task type: STREAM
        Seed: 2
}