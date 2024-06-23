# GEGELATI Applications
This project is tested with BrowserStack
The purpose of this repository is to gather several applications built with the [GEGELATI library](https://github.com/gegelati/gegelati).

## Available applications
* [MNIST](./mnist): Classification learning environment based on the eponymous data-set from LeCun et al.
* [Pendulum](./pendulum): Physical environment with an oscillating pendulum to be stabilized upward.
* [Stick Game](./stickgame): Learning environment for the Nim strategy game known as the "21 game".
* [Tic Tac Toe](./tic-tac-toe): Thrilling ancestral strategy game for two players.

## Continuous Integration

| Branches | [CI  Status](https://github.com/gegelati/gegelati-apps/actions/workflows/ci.yml) |
| ------------- |  ------------- |
| master  |  [![CI](https://github.com/gegelati/gegelati-apps/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/gegelati/gegelati-apps/actions/workflows/ci.yml)  |
| develop  | [![CI](https://github.com/gegelati/gegelati-apps/actions/workflows/ci.yml/badge.svg?branch=develop)](https://github.com/gegelati/gegelati-apps/actions/workflows/ci.yml) |

All applications are currently tested under a Windows environment with MSVC19, and under a Linux environment with gcc 9.

## License

This project is distributed under the CeCILL-C license (see LICENSE file).
