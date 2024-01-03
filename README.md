# RV-Monitors
A `C++17` implementation of several temporal logic runtime verification algorithms [[1]](#1), [[2]](#2) for LTLf, PLTLf, and MTL formulas on finite traces:
- Dynamic Programming
- Event Consuming
- MTL Event Consuming
- Multi-transition Finite State Machine (MT-FSM*)
- Binary Transition Finite State Machine (BTT-FSM*)
- Multi-Head Monitoring (Hydra)
- Extended Multi-Head Monitoring

### Overview
We extracted the relevant information in separate readmes depending on the way you want to use the CommonRoad Monitor:

- You want to integrate the RV-Monitor into another CMake project: [C++ Usage](./../docs/cpp.md)
- You want to set up a development environment for the RV-Monitor itself: [C++ Development](https://gitlab.lrz.de/cps/commonroad/environment-model/-/blob/develop/docs/cpp_dev.md?ref_type=heads)
- You want to learn about the evaluation of our monitor against Hydra/Vydra inside the CI: [C++ Usage](./../docs/vydra.md)
- You want to generate the docu of the RV-Monitor: [Documentation Generation](https://gitlab.lrz.de/cps/commonroad/environment-model/-/blob/develop/docs/doc.md?ref_type=heads)


## References
<a id="1">[1]</a> 
G. Roşu and K. Havelund. “Rewriting-based techniques for runtime verification.”
In: Automated Software Engineering 12.2 (2005), pp. 151–197.

<a id="2">[2]</a>
M. Raszyk, D. Basin, D. Traytel. “Multi-head Monitoring of Metric Dynamic Logic.”
In: Automated Technology for Verification and Analysis (ATVA) 2020, pp. 233–250.
