# C++ Actor Framework Supervisor

This project shall demonstrate the capabilities of a reactive software design with the C++ Actor Framework (CAF)
on the example of a lightweight C++ supervisor process that monitors another lightweight C++ process and restarts it in case of an error.

<div align="center">
<img src="https://github.com/weberdaniel/supervisor-demo/raw/main/doc/images/restart.png" width="50%" alt><br>
<em>Child process is being restarted by the supervisor</em>
</div>

## Build & Run:

\# 1. Optional: define proxy <br>
$> export http_proxy=  <br>
$> export https_proxy=  <br>

\# 2. Recommended: define number of threads (not more than cpus!) and do parallel build  <br>
$> export number_of_build_jobs= <br>
$> export DOCKER\_BUILDKIT=1  <br>

\# 3. get it <br>
$> git clone https://github.com/weberdaniel/caf-supervisor

\# 4. build & run it <br>
$> cd caf-supervisor <br>
$> docker compose build  <br>
$> docker compose up <br>

afterwards you can login to the Grafana dashboard in your browser via

http://localhost:3000 <br>

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
<td> The principle of reactive software design patterns reaches back to the 1970s. A famous example of a reactive software design is the AXD301 Ericsson switch, which provides a nine-nines reliability (99.9999999%), which is basically a millisecond of downtime per year. Since highly distributed software architectures are very common today - reactive design patterns have been rediscovered (see Docker Swarm) but can also become relevant on hardware related software domains as well (performance): Examples for this is the IoT. Multi-core CPUs can be regarded as distributed systems as well.. Since today any kind of software can be regarded as a part of a distributed architecture, reactive design patterns can be applied to almost any software on any OSI layer. This is reflected in the creation of the Reactive Manifesto in order to spread word about the design concepts: https://www.reactivemanifesto.org
(Book Cover: Reactive Design Patterns, Roland Kuhn)  </td>
</tr>
</tbody>
</table>

### An example design pattern: "Crash me if you can"
  
The 'Let it crash'/'Crash me if you can' pattern is one of the elementary design patterns of the domain. In order to build fault-tolerant software, a common design pattern is to  only code for the application usecase - in any other usecase - the process will just crash and be restarted. There are various restart strategies, which can be looked up at the definition in Erlang which is given here: https://www.erlang.org/doc/man/supervisor.html
  
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

The goal of this project is to implement a supervision hierarchy inside a C++ high performance application.
  
## Limitations

simple_one_for_one strategy is missing. Only works with function object based actors.
    
## Contribution  

In SCRUM you usually find a DEFINITION OF DONE, telling what is expected of the code quality. In this case there is no scrum team, but still a Definition of Done is used as far as possible.

### The Definition of Done:
  
- Fullfillment of Google Coding Style (cpplint must give 0 errors)
- C++ Core Guidelines must be kept : https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
- Another Person must review the code
- Unit Test must be > 80%
- Memory Check with Valgrind
- Code Coverage > 80%

