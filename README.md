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

### "Crash me if you can": Restart Strategies
  
There are various restart strategies, which can be looked up at the definition in Erlang which is given here: https://www.erlang.org/doc/man/supervisor.html
    
## Aims and Goals

The goal of this project is to implement a supervision hierarchy inside a C++ high performance application.
  
## Limitations

simple_one_for_one strategy is missing. Only works with function object based actors.
    
## Contribution  

Stick to Google Coding Stylguide and C++ Core Guidelines!

