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
# Speed makes up for the high power usage.
#
# Specs:
#       Max Power Consumption: 2228 W/h
#       MIPS/Power: 63.2
machine class:
{
        Number of machines: 2
        CPU type: X86
        Number of cores: 64
        Memory: 65536
        S-States: [180, 120, 120, 90, 50, 15, 0]
        P-States: [20, 15, 10, 5]
        C-States: [12, 4, 2, 0]
        MIPS: [2200, 1500, 1000, 500]
        GPUs: no
}

# POWER8 borrowed from a friend.
#
# Specs:
#       Map Power Consumption: 316
#       MIPS/Power: 30.4
machine class:
{
        Number of machines: 1
        CPU type: POWER
        Number of cores: 6
        Memory: 16384
        S-States: [160, 110, 110, 85, 45, 12, 0]
        P-States: [14, 10, 7, 4]
        C-States: [12, 3, 1, 0]
        MIPS: [1600, 1200, 800, 400]
        GPUs: yes
}

# Some Raspberry Pi Pico 2 (RP2350) purchased to support RISCV requests.
#
# Specs:
#       Map Power Consumption: 24
#       MIPS/Power: 41.7
machine class:
{
        Number of machines: 8
        CPU type: RISCV
        Number of cores: 2
        Memory: 520
        S-States: [8, 6, 6, 4, 2, 1, 0]
        P-States: [5, 3, 2, 1]
        C-States: [3, 2, 1, 0]
        MIPS: [1000, 600, 300, 150]
        GPUs: no
}


# General machines with few cores. Apple M1 MacBook Air Laptops bought at
# sharp discount from people upgrading to M3. To simulate the 4 performance and
# 4 efficiency cores, two separate machines are used.
# Performance cores
#
# Specs:
#       Map Power Consumption: 200 W/h
#       MIPS/Power: 28
machine class:
{
        Number of machines: 8
        Number of cores: 4
        CPU type: ARM
        Memory: 10240
        S-States: [120, 100, 100, 80, 40, 10, 0]
        P-States: [12, 10, 6, 4]
        C-States: [8, 4, 1, 0]
        MIPS: [1400, 1000, 700, 300]
        GPUs: yes
}

# Efficiency cores
#
# Specs:
#       Map Power Consumption: 146 W/h
#       MIPS/Power: 24.7
machine class:
{
        Number of machines: 8
        Number of cores: 4
        CPU type: ARM
        Memory: 6144
        S-States: [90, 70, 70, 50, 25, 10, 0]
        P-States: [8, 6, 4, 2]
        C-States: [6, 3, 1, 0]
        MIPS: [900, 600, 400, 200]
        GPUs: no
}

# Some standard X86 machines that have lower max power usage compared to those
# high performance machines.
#
# Specs:
#       Map Power Consumption: 232 W/h
#       MIPS/Power: 31
machine class:
{
        Number of machines: 4
        CPU type: X86
        Number of cores: 8
        Memory: 16384
        S-States: [120, 100, 100, 80, 40, 10, 0]
        P-States: [8, 6, 4, 2]
        C-States: [6, 3, 1, 0]
        MIPS: [900, 600, 400, 200]
        GPUs: yes
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