# pie
@author Raphael De Oliveira

1)  Clone this git with the command : git clone https://github.com/raph5640/pie
2)  Compile the main.c with the command : gcc -o pie main.c -lgd -lm 

3) Launch the program pie using a terminal with the following command: ./pie Tokyo=20 London:18 UnitedStates=12 Paris:12
4) You must provide arguments to the program in the following format: ./pie string\<separator>integer  string\<separator>integer  string\<separator>integer
5) The \<separator> can be either "=" or ":".

!! WARNING !! : if you execut pie and don't give him any arguments it will stop the program with exit 1 and a WARNING. (Likewise if the sum of the percentages is strictly greater than 100)
