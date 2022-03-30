# A supervisor demo of C++ Actor Framework (CAF)

This project shall demonstrate the capabilities of the C++ Actor Framework (CAF)
on the example of a lightweight C++ supervisor process that monitors another lightweight 
C++ process and restarts it in case of an error.

## Reactive Software Design

<img align='left' src="https://github.com/weberdaniel/supervisor-demo/blob/main/doc/images/reactive_design.jpg" width="230">

The principle of reactive software design patterns reach back to the 1970s. An example of a reactive software design is the AXD301 Ericsson switch, which provides a nine-nines reliability (99.9999999%). Reactive design patterns become more important again.  In particular due to highly distributed software architectures they gain relevance. But also because a multi-core CPUs behaves like a distributed system as well. The design principles of devices like the Ericsson switch become more and more adopted to new and old programming domains, so the Reactive Manifesto has been created to spread the design concepts: https://www.reactivemanifesto.org

## The 'Let it crash' pattern

## A C++ Implementation: The C++ Actor Framework (CAF)

https://www.actor-framework.org <br>
https://github.com/actor-framework/actor-framework.git
