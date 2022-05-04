# A supervisor demo of C++ Actor Framework (CAF)

This project shall demonstrate the capabilities of the C++ Actor Framework (CAF)
on the example of a lightweight C++ supervisor process that monitors another lightweight 
C++ process and restarts it in case of an error.

## Reactive Software Design

<img align='left' src="https://github.com/weberdaniel/supervisor-demo/blob/main/doc/images/reactive_design.jpg" width="230">

The principle of reactive software design patterns reaches back to the 1970s. A famous example of a reactive software design is the AXD301 Ericsson switch, which provides a nine-nines reliability (99.9999999%). Since highly distributed software architectures are common - reactive design patterns have been rediscovered and become relevant on hardware related software domains as well. Examples for this are the IoT. But also tech companies aim for a decentralized fault-tolerant data center technology. For example Microsoft is testing out underwater distributed data centers. Multi-core CPUs can be regarded as distributed systems as well. Therefore The design principles of devices like the Ericsson switch are getting rediscovered and become implemented on other programming domains as well. Since today any kind of software can be regarded as a part of a distributed architecture, reactive design patterns can be applied to almost any software that should be fault-tolerant and highly reliable. This is reflected in the creation of the Reactive Manifesto in order to spread word about the design concepts: https://www.reactivemanifesto.org
(Book Cover: Reactive Design Patterns, Roland Kuhn)

## The 'Let it crash' pattern

<img align='right' src="https://github.com/weberdaniel/supervisor-demo/blob/main/doc/images/crash-me-if-you-can.png" width="350">
The 'Let it crash' pattern is one of the elementary design patterns of the domain. In order to build fault-tolerant software, a common design pattern is to  only code for the applcation usecase - in any other usecase - the process will just crash and be restarted.  


## A C++ Implementation: The C++ Actor Framework (CAF)

https://www.actor-framework.org <br>
https://github.com/actor-framework/actor-framework.git
