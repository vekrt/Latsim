[![C/C++ CI](https://github.com/kevintyloo/Latsim/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/kevintyloo/Latsim/actions/workflows/c-cpp.yml)

# Latsim

<p float="left", align="center">
  <img src="https://user-images.githubusercontent.com/49154901/113190011-26c4a480-925c-11eb-947d-e07a9a173c70.png" width="300" />
  &nbsp;&nbsp;&nbsp;&nbsp;
  <img src="https://user-images.githubusercontent.com/49154901/113180001-87e67b00-9250-11eb-99c8-26eca989bf41.png" width="300" /> 
</p>

## Quick start
On Ubuntu in terminal
```` console
sudo apt-get install libglfw3 libglfw3-dev libxi-dev
make main_opengl
./main_opengl config_opengl.in
````
If you don't want the visualisation then run `make main` then `./main config.in threads=2` for instance and it will launch simulations with different inverse temperature.

## Lattice and dual lattice
Three kind of lattices are considered: square, honeycomb and triangular (black lines). The spins (circles) are placed on the intersections. Each lattice has a dual lattice (dashed blue lines) where the spins are centered in the plaquette. The dual lattice representation is nicer to display in the visualisation.

![schema](https://user-images.githubusercontent.com/49154901/113319108-66999380-9311-11eb-8943-d034e440860d.png)

## Models
![model](https://user-images.githubusercontent.com/49154901/113325870-3fdf5b00-9319-11eb-865b-f8bdd917f8ba.png)

## Simulation methods

[Metropolis algorithm](https://en.wikipedia.org/wiki/Ising_model) and [Wolff clustering algorithm](https://arxiv.org/pdf/cond-mat/0311623.pdf)

## Binder cumulant
[Binder cumulant](https://en.wikipedia.org/wiki/Binder_parameter) allows to determine the critical temperature by finding the crossing point between simulations with different lattice size *L*.

![binder](https://user-images.githubusercontent.com/49154901/113342796-3660ed80-932f-11eb-8b6d-176afcc7717f.png)

Binder cumulant for the Ising model, the dashed black line shows the critical temperature found in the [litterature](http://cds.cern.ch/record/746866/files/0406135.pdf).

## Visualisation

Each plaquette of the dual lattice is rendered on screen therefore the performance becomes poorer as the size of the lattice grows. Furthermore, updates of the Wolff cluster algorithm can be slow for large lattices.

Key             |  Effect
------------    | -------------
`AWSD`          | Left - Up - Down - RIght
`H`             | Reset view
`Page Up/Down`  | Zoom In/Out
`M/L`           | More/less simualtion updates per frame


## TODOs

* [Feedback-optimized parallel tempering Monte Carlo](https://arxiv.org/abs/cond-mat/0602085) is implemented but not working for now
* Clean the code
* Implement Kagome lattice
* Increase the palette size to visualise q larger than 4

## Reference
* [Entropy production at criticality in a nonequilibrium Potts model](https://iopscience.iop.org/article/10.1088/1367-2630/abb5f0/pdf) Thomas Martynec et al 2020 New J. Phys. 22 093069
* [Monte Carlo Simulations of the 3-StatePotts Model in 2D](https://esc.fnwi.uva.nl/thesis/centraal/files/f109745660.pdf) Teun Zwart 2016
* [Calculation of the Critical Temperature for 2- and 3-Dimensional Ising Models and for2-Dimensional Potts Models Using the Transfer Matrix Method](http://cds.cern.ch/record/746866/files/0406135.pdf) J. Phys. Chem. B 2001, 105, 42, 10355–10359
* [Cluster Monte Carlo algorithms](https://arxiv.org/abs/cond-mat/0311623v1) Werner Krauth 2003
