# simpix

## To compile
In the phys56xx environment, use ```make```, and all neeeded files will be built. the main executable is ```simpix```. 

## Images
The two images I switched are labeled ```impressionism_1.png``` and ```impressionism_2.png```. The switched images are ```impressionism_1_to_2.png``` and ```impressionism_2_to_1.png```, where the images were transformed 1=>2 and 2=>1, repsectivley. You can also see this in the images ```collage_1_to_2.png``` and ```collage_2_to_1.png```, where the images are displayed as the source (top left), target (top right), and result (bottom left). 

## Structure of the script
The executable is in ```simpix.cpp```. Here, the images are loaded using the ```TASImage``` class. The annealing steps are performed by the Annealing class, whihc is defined in ```Utils.hpp/cpp```. The function which determines the _difference_ betweeen two pixels is also in ```Utils.hpp/cpp```. The Annealer class actually does the pixel-swap annealing steps, while the temperature cooling schedule is performed by the ```simpix.cpp``` executable.  

## To Execute
Once you have compiled the ```simpix``` executable, you can run with the following options: 

- ```-h``` Print a message listing possible options, and exit. 
- ```-t``` Provide a path to the 'target' image 
- ```-s``` Provide a path to the 'source' image 
- ```-o``` Provide a path to save the output image
- ```-c``` Provide a path to save the 'collage' of all 3 images 

## Examples of Execution + Measured Execution Time
As an example, to transform ```impressionism_2.png``` to ```impressionism_1.png```, you can use: 

``` 
./simpix -t impressionism_1.png -s impressionism_2.png -o impressionism_2_to_1.png -c collage_2_to_1.png
```

Which takes **47.16 s** to execute with the current annealing parameters. 

Going the other way, you can use: 

``` 
./simpix -t impressionism_2.png -s impressionism_1.png -o impressionism_1_to_2.png -c collage_1_to_2.png
``` 

Which takes **45.64 s** to execute. 




