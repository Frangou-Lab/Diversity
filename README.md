Diversity
=========
Diversity indices calculator for comma- or tab-separated tables. The list of applied measures:
> Simpsons index (and its recoprocal, approximation)  
> Dominance index (and its approximation)  
> Shannon index (log, log2, log10)  
> Menhnick index  
> Buzas-Gibson's index  
> Equitability index  
> Alternate reciprocal Simpsons  
> Berger-Parker dominance  
> Margalef's richness index  
> Rényi entropy  
> Gini coefficient

Usage
-----
    ./diversity --by-rows input_table.csv

    ./diversity --by-columns input_table.csv

    ./diversity --by-rows --by-columns input_table.csv

Building
--------
The application requires Clang 3.5 or later. Any other C++14 supporting compiler will do as well, but
you will need to adjust 'source/Makefile'.

    make

Acknowledgements
----------------
* Gus Frangou
* [Roswell Park Cancer Institute](https://www.roswellpark.org/)
* Denis Musatov, [Axmor Software – America, Inc.](https://axmor.com/)

License
-------
This software is released under the Apache 2.0 license, a copy of which is located in the LICENSE file.
