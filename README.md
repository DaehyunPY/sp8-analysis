# sp8-analysis

## How to run this program
### Method 1: Compile the program
Execute `mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make all` on terminal
then `sp8sort` and `sp8ana` will be found in `build` folder. Plus, you can install it 
to execute `sudo make install`.

### Method 2: Use docker
Simply execute `sort.sh` or `ana.sh` shell scripts. Don't forget to modify few lines in the scripts. 

## How to use ROOT macros 
Copy `usr` folder and `rootlogon.C` file to your ROOT macros folder. It might be `/usr/share/root/macros`.
