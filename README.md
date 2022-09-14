# Supervisor for C++ Actor Framework (CAF)

<div align="center">
<img src="https://github.com/weberdaniel/supervisor-demo/raw/main/doc/images/crash-me-if-you-can.png" alt><br>
<em>License: CC-BY-NC-ND by Fred Hebert (www.learnyousomeerlang.com)</em>
</div>

<br>

This project shall demonstrate the capabilities of the C++ Actor Framework (CAF)
on the example of a lightweight C++ supervisor process that monitors another lightweight 
C++ process and restarts it in case of an error. This shall resemble the capabilities
of an Erlang Supervisor - which has different supervising strategies (e.g. one_for_one) 
and can be used to span complex and distributed supervision hierarchies (called applications).

# Build & Run:

## Build & Run Locally (Recommended | Without additional Download | Tested on Linux Ubuntu 20.04)

### Step A: Install libunwind 

First please note, i have created a fork of C++ Actor Framework which uses libunwind instead of
libexecinfo. It is not a big change, but it enables to use C++ Actor Framework development mode on
Alpine Linux. Still though, libubsan and libasan are not supported on Alpine so you cant use
address sanitizers and undefined behaviour sanitizers.

$ sudo apt-get install libunwind-dev <br>

### Step B: Build and Download external dependencies:

$ git clone https://github.com/weberdaniel/caf-supervisor <br>
$ cd caf-supervisor <br>
$ mkdir build <br>
$ cd build <br>
$ cmake .. <br> 
$ make -j 24 # in case of 24 cpu cores <br>

### Step C: Repeat to create executables:

$ cmake .. <br> 
$ make -j 24 # in case of 24 cpu cores <br>

### Step D: Run:

$ cd .. <br>
$ ./build/app/svdemo/svdemo --config-file=./app/svdemo/caf-application.conf

The Config file can be omitted, but then you will not see anything, since you will
not get Debugging messages.

## Use Github Package (Not Recommended | High Download | Platform Independent)

$ docker pull ghcr.io/weberdaniel/caf-supervisor:latest <br>
$ docker run ghcr.io/weberdaniel/caf-supervisor

## Build & Run in Docker Alpine Linux Container ( Not Recommended | High Download | Platform Independent  )
 
$ git clone https://github.com/weberdaniel/caf-supervisor <br>
$ cd caf-supervisor <br>
$ export DOCKER\_BUILDKIT=1 <br>
$ docker build --no-cache --build-arg number\_of\_build\_jobs=23 -t svdemo . <br>
$ docker run svdemo

The DOCKER\_BUILDTKIT purpose is to enable parallel build, so all CPU cores (minus one) will be working in the build process. You can also leave out the BUILDKIT option and only do

$ docker build -t svdemo .

but this will take very long. Also take care not to use more jobs than you have cores on your machine and always leave some room for other applications on the machine, otherwise you might run out of memory.

Also if you use BUILDKIT, never forget the number\_of\_build\_jobs argument, otherwise your machine will crash since it will use an unlimited amount of build jobs.

## Build & Run with Docker Compose (Ongoing)

Alternativly to specifing the build-args on the command line, you can specify identical environment variables in for example .bashrc:

docker compose build --no-cache --build-arg number_of_build_jobs=23 --build-arg http_proxy=http://URL:PORT --build-arg https_proxy=http://URL:PORT

## Actor Frameworks and Reactive Design

Actor Frameworks are a crucial building block for Artificial Intelligence. Actor Frameworks have been first introduced by 
Carl Hewitt, Peter Bishop und Richard Steiger in 1973.

### The C++ Actor Framework

<table>
<tbody>
<tr>
<td width="33%"> <div align="center"><img src="https://www.actor-framework.org/static/img/caf-logo.png" width="200"> </div> </td>
<td> A notable modern implementation of an Actor Framework in C++ is CAF, the C++ Actor Framework (https://www.actor-framework.org). In contrary to programming middleware like CORBA or ACE, it uses modern C++ to improve memory management, performance and type safety. </td>
</tr>
</tbody>
</table>

Some highlights:

- leverage on C++ high performance
- low memory footprint
- native bindings to Prometheus DB for high-performance data science in real-time
- Resilience due to Reactive Design
- Scalability
- Tight memory control
- modern c++
- used for high-performance network analysis (Zeek)
- Actors blend into Qt Widgets seamlessly by deriving from caf::mixin::actor_widget<QWdiget> 

### Reactive Software Design: Make your software fault tolerant!
  
<table>
<tbody>
<tr>
<td width="33%"> <img align='left' src="https://github.com/weberdaniel/supervisor-demo/raw/main/doc/images/reactive_design.jpg" width="100%">  </td>
<td> The principle of reactive software design patterns reaches back to the 1970s. A famous example of a reactive software design is the AXD301 Ericsson switch, which provides a nine-nines reliability (99.9999999%), which is basically a millisecond of downtime per year. Since highly distributed software architectures are common - reactive design patterns have been rediscovered and become relevant on hardware related software domains as well. Examples for this are the IoT. But also tech companies aim for a decentralized fault-tolerant data center technology. For example Microsoft is testing out underwater distributed data centers. Multi-core CPUs can be regarded as distributed systems as well. Therefore the design principles of devices like the Ericsson switch AXD301 are getting rediscovered and become implemented on other programming domains as well. Since today any kind of software can be regarded as a part of a distributed architecture, reactive design patterns can be applied to almost any software that should be fault-tolerant and highly reliable. This is reflected in the creation of the Reactive Manifesto in order to spread word about the design concepts: https://www.reactivemanifesto.org
(Book Cover: Reactive Design Patterns, Roland Kuhn)  </td>
</tr>
</tbody>
</table>

### An example design pattern: "Crash me if you can"
  
The 'Let it crash'/'Crash me if you can' pattern is one of the elementary design patterns of the domain. In order to build fault-tolerant software, a common design pattern is to  only code for the applcation usecase - in any other usecase - the process will just crash and be restarted. The result looks like the following:

<br><br>
<div align="center">
<img src="https://github.com/weberdaniel/supervisor-demo/raw/main/doc/images/restart.png" width="50%" alt><br>
<em>Child process is being restarted by the supervisor</em>
</div>
<br>
  
The Erlang specification of a supervisor is slightly more complex and incorporates various strategys on how to recover an array of processes. For example if one child process crashes all need to be restarted. To understand what i build here in C++, you can look at the definition in Erlang which is given here: https://www.erlang.org/doc/man/supervisor.html
  
#### One for One Strategy
  
<br><br>
<div align="center">
<img src="https://github.com/weberdaniel/supervisor-demo/raw/main/doc/images/sup-one_for_one.png" width="50%" alt style="fill: blue"><br>
</div>
  
#### One for All Strategy
<br><br>
<div align="center">
<img src="https://github.com/weberdaniel/supervisor-demo/raw/main/doc/images/sup-one_for_all.png" width="50%" alt><br>
</div>
  
#### Rest for One Strategy
<br><br>
<div align="center">
<img src="https://github.com/weberdaniel/supervisor-demo/raw/main/doc/images/sup-rest_for_one.png" width="50%" alt><br>
</div>


## Aims and Goals

The goal of this project is to implement an Erlang supervision hierarchy inside a C++ high performance application, as closly as possible resembling the principles of Erlang, but in the domain of high performance computing. 
  
## Limitations

Currently one_for_one, one_for_all, rest_for_one strategy is implemented,
but not simple_one_for_one. Also currently this only works with function object 
(functor) based actors, not class based actors or function based actors.
    
## Contribution  

In SCRUM you usually find a DEFINITION OF DONE, telling what is expected of the code quality. In this case there is no scrum team, but still a Definition of Done is used as far as possible (Reviews are not always possible) - still i have to admit - whithout a proper review process working alone will never be comparable to working in a SCRUM team.

### The Definition of Done:
  
- Fullfillment of Google Coding Style (cpplint must give 0 errors)
- C++ Core Guidelines must be kept : https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
- Another Person must review the code
- Unit Test must be > 80%
- Memory Check with Valgrind
- Code Coverage > 80%

