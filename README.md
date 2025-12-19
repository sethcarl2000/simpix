# simpix

## To compile
In the phys56xx environment, use ```make```, and all neeeded files will be built. the main executable is ```simpix```. 

## Images
The two images I switched are labeled ```impressionism_1.png``` and ```impressionism_2.png```. The switched images are ```impressionism_1_to_2.png``` and ```impressionism_2_to_1.png```, where the images were transformed 1=>2 and 2=>1, repsectivley. You can also see this in the images ```collage_1_to_2.png``` and ```collage_2_to_1.png```, where the images are displayed as the source (top left), target (top right), and result (bottom left). 

## Structure of the script. 
The executable is in ```simpix.cpp```, and several helper functions, as well as the 'Annealer' class, are in ```Utils.hpp/cpp```. The actual Annealing code, and the function which determines the _difference_ betweeen two pixels, are in ```Utils.hpp/cppo```. Th Annealer class actually does the pixel-swap annealing steps, while the temperature is exponnetially decreased in the ```simpix.cpp``` class. 

## To Execute
Once you have compiled the ```simpix``` executable, you can run with the following options: 

- ```-h``` Print a message listing possible options, and exit. 
- ```-t``` Provide a path to the 'target' image 
- ```-s``` Provice a path to the 'source' image 
- ```-o``` Provide a path to save the output image
- ```-c``` Provide a path to save the 'collage' of all 3 images 

## Examples of Execution / Time Needed
As an example, to transform ```impressionism_2.png``` to ```impressionism_1.png```, you can use: 

``` 
./simpix -t impressionism_1.png -s impressionism_2.png -o impressionism_2_to_1.png -c collage_2_to_1.png

```bash 

Which takes **47.16 s** to execute with the current annealing parameters. 

Going the other way, you can use: 

``` 
./simpix -t impressionism_2.png -s impressionism_1.png -o impressionism_1_to_2.png -c collage_1_to_2.png

```bash 

Which takes **44.20 s** to execute. 




