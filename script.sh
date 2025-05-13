#!/bin/bash

make

./init

gnome-terminal -- bash -c "./castor; exec bash"

gnome-terminal -- bash -c "./viewer 1; exec bash"
gnome-terminal -- bash -c "./viewer 2; exec bash"
gnome-terminal -- bash -c "./viewer 3; exec bash"

gnome-terminal -- bash -c "./tester; exec bash"

./cleanup

make clean
