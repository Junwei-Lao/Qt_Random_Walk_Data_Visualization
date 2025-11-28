This is the desktop application intended to use Qt to implement the data visualization tool for random walk trajectories.

## Naming rules:

1. **Data is stored in csv file form**

2. **Files should start with their classification:**

  1. **If the csv file stores the mixing time from one case, start with _SingleDistribution_**

    1. Then the name of the file should be:  
       SingleDistribution_<Shape>_<Index>_<Order>_<Initial Temperature>_<Final Temperature>.csv
    2. Shape: The shape we are investigating: Circle, Rectangle, etc.
    3. Index: The number to standardize the area
    4. Order: The power of the distance to calculate weights of each direction
    5. Initial temperature: The parameter used to calculate the weight of each direction
    6. Final temperature: The parameter used to calculate the weight of each direction

  2. **If the csv file stores the results from all the cases, start with _GeneralDistribution_**

    1. Then the name of the file should be:  
       GeneralDistribution_<Shape>_<Index>_<Order>_<Initial Temperature>_<Final Temperature>.csv
    2. Shape: The shape we are investigating: Circle, Rectangle, etc.
    3. Index: The number to standardize the area
    4. Order: The power of the distance to calculate weights of each direction
    5. Initial temperature: The parameter used to calculate the weight of each direction
    6. Final temperature: The parameter used to calculate the weight of each direction

  3. If the csv file stores the walking path from one case, start with _SingleWalk_

    1. Then the name of the file should be:  
       SingleWalk_<Shape>_<Index>.csv
    2. Shape: The shape we are investigating: Circle, Rectangle, etc.
    3. Index: The number to standardize the area

## Intended Design:

1. The single case distribution from one shape/index/etc. should be allowed to compare with the other shape/index/etc. in the histogram way.
2. The general case distribution should also be allowed to compare.
3. Meta data should be shown at the top of the chart.
4. The walk will be compressed into ~1000 points for better view.
