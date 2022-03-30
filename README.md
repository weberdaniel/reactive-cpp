# A supervisor demo of C++ Actor Framework (CAF)

This project shall demonstrate the capabilities of the C++ Actor Framework (CAF)
on the example of a lightweight C++ supervisor process that monitors another lightweight 
C++ process and restarts it in case of an error.

## Reactive Software Design

<img align='left' src="https://github.com/weberdaniel/supervisor-demo/blob/main/doc/images/reactive_design.jpg" width="230">

The principle of reactive software design patterns reaches back to the 1970s. A famous example of a reactive software design is the AXD301 Ericsson switch, which provides a nine-nines reliability (99.9999999%). In particular due to highly distributed software architectures reactive design patterns started to gain relevance again, for example Microsoft testing out underwater distributed data centers. Also multi-core CPUs and the hardware domain (IoT) can be regarded as a distributed system. The design principles of devices like the Ericsson switch therefore have been rediscovered and become more and more adopted and integrated into new and old programming domains. Since any kind of software nowadays is part of a distributed architecture usually, all software should be fault-tolerant and highly reliable. Therefore the Reactive Manifesto has been created to spread word about the design concepts: https://www.reactivemanifesto.org

## The 'Let it crash' pattern

The 'Let it crash' pattern is one of the elementary design patterns. In order to build fault-tolerant software, only code for the usecase and let the process crash in all other cases, to then restart it by a supervisor. It is one of the base patterns for an automated and fault-tolerant software system.

## A C++ Implementation: The C++ Actor Framework (CAF)

https://www.actor-framework.org <br>
https://github.com/actor-framework/actor-framework.git
