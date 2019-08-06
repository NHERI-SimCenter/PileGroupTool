[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.1410668.svg)](https://doi.org/10.5281/zenodo.1410668)

![missing PGT logo][logo]

[logo]: https://nheri-simcenter.github.io/PileGroupTool/docs/NHERI-PGT-Icon.png "Pile Group Tool Logo"

# Pile Group Tool

The SimCenter Pile-Group Tool provides a dynamic interface to study the behavior of a pile or pile-group in layered soil. It allows the user to interactively (and nearly instantly) observe the system's response to changes of the following parameters:

* soil structure (up to 3 layers of variable thickness) and soil properties
* position of the ground water table
* pile stiffness, embedment length, connection type to the pile cap
* number of piles in the group (one to three piles)
* the applied horizontal and vertical forces and moment at the pile cap (push-over analysis)

## Presented results include:

* axial and lateral displacement of all piles
* moment, shear and force diagrams for each pile
* vertical soil stresses
* ultimate strength and stiffness parameters for the employed p-y springs

## Technical background:

The soil-structure system is modeled using a finite-element platform. Piles are represented by displacement based beam-column elements. Soil-structure interaction is represented laterally by p-y springs and tangentially by t-z springs. Spring properties are computed using Hansen's method [1] and recommendations by the American Petroleum Institute (API) [2]. The Pile-Group Tool constructs a structurally and numerically appropriate finite-element mesh, computes spring properties from soil properties, and places spring elements accordingly. The actual simulation employs a Newton-Raphson iterative procedure to solve the governing nonlinear system of equations.

## Downloads

* __download a binary version__ of the latest release on [simcenter.designsafe-ci.org/learning-tools/pile-group-tool/](https://simcenter.designsafe-ci.org/learning-tools/pile-group-tool/)
* __download the source__ of latest release at [github.com/NHERI-SimCenter/PileGroupTool/releases](https://github.com/NHERI-SimCenter/PileGroupTool/releases)

## Software class documentation

* Documentation main page: [https://nheri-simcenter.github.io/PileGroupTool/](https://nheri-simcenter.github.io/PileGroupTool/)
* An (as-yet-incomplete) documentation of the code can be found at: [nheri-simcenter.github.io/PileGroupTool/docs/index.html](https://nheri-simcenter.github.io/PileGroupTool/docs/index.html)
* view the source on [github.com/NHERI-SimCenter/PileGroupTool](https://github.com/NHERI-SimCenter/PileGroupTool)

Please visit the [Pile Group Tool webpage](https://simcenter.designsafe-ci.org/learning-tools/pile-group-tool/)
for more resources related to this tool. Additionally, this page
provides more information on the NHERI SimCenter, including other SimCenter
applications, FAQ, and how to collaborate.

# References:

1. Brinch Hansen, J. (1961). “The ultimate resistance of rigid piles against transversal forces.”Bulletin No. 12, Geoteknisk Institute, Copenhagen, 59.
2. American Petroleum Institute (API) (1987). Recommended Practice for Planning, Designing and Constructing Fixed Offshore Platforms. API Recommended Practice 2A(RP-2A), Washington D.C, 17th edition.
