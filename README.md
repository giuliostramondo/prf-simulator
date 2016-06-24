Description                         {#mainpage}
============

The Polymorphic Register File (PRF) are memory modules designed in order to allow fast parallel access to matrices in high performance applications. Through the use of mapping functions ( m_v(int,int,scheme,int,int), m_h(int,int,scheme,int,int), A_standard(int,int,int,int)} ) an N-Dimensional matrix is stored in an N+1-Dimensional structure enabling, for certain type of matrix accesses, a faster retrival of the data. The PRF are implemented using an array of independent memory modules that can be read and written in parallel. There are multiple ways in which the data can be organized, those will be referred to as *Access Schemes*. Each Access Scheme allows to read the data stored in the PRF - in a conflict free manner - with different shapes, called *Access Types*. Assuming that the PRF is bidimensional and that is implemented using p*q independent memory modules organized in a pxq matrix, all the possible parallel access type supported by a PRF are the following: 

Access Type    | Description
-------------- | -------------
Rectangular                | pxq sub-matrix rectangle of the original matrix.
Row                        | p*q element of a row of the original matrix.
Column                     | p*q element of a column of the original matrix.
Secondary Diagonal         | p*q element of a main diagonal of the original matrix.
Transposed Rectangular     | qxp sub-matrix rectangle of the original matrix.

Once that the original input matrix is stored in a PRF using an access scheme, it will be possible to perform conflict free parallel accesses with one or more access types. The table below shows what access type are available for each access scheme.


Access Type    | Description
-------------- | -------------
Rectangle only             | Allows rectangular conflict free accesses.
Rect&Row                   | Allows rectangular, row, main diagonal and secondary diagonal conflict free access.
Rect&Col                   | Allows rectangular, column, main diagonal and secondary diagonal conflict free access.
Row&Col                    | Allows rectangular, row and column conflict free access.
Rect\&Trect                | Allows rectangular and transposed rectangular conflict free access.

This simulator was written to allow a easier visualization of the effect of each access scheme and to provide a platform for the exploration of the access schemes.

The simulator is implemented following the description given in
["On implementability of Polymorphic Register Files"]: http://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=6322873&tag=1 "On implementability of Polymorphic Register Files"

Installation
============
The sources for the program are available on git ( once obtained the permission from the owner of the repository ). 

To download the code execute the following line from the terminal.

~~~~~~~~~~~~~~~~~~~~~
git clone https://github.com/giuliostramondo/prf-simulator.git
~~~~~~~~~~~~~~~~~~~~~

To compile the sources 
~~~~~~~~~~~~~~~~
cd prf_simulator
make
~~~~~~~~~~~~~~~~

This produces an executable called prf\_console in the ./bin folder.

Usage
======

Usage: ./prf [Options]


-N \<num\>	 Change the horizontal size of the input matrix (default 9)

-M \<num\>	 Change the vertical size of the input matrix (default 9)

-p \<num\>	 Change the horizontal size of the PRF (default 3)

-q \<num\>	 Change the horizontal size of the PRF (default 3)

-s \<num\>	 Change the schema used by the PRF (default 0 -> RECTANGLE_ONLY)
	  		other schemes :

Access Scheme   | Description
----------------| -------------
0               | Rectangle only           
1               | Rect&Row                   
2               | Rect&Col                   
3               | Row&Col                    
4               | Rect\&Trect               

Once that the prf\_console is executed, an input matrix NxM is created and its cells are populated with different cells identifier. A PRF with p*q different memory modules organized in a pxq matrix is instantiated, the memory modules are populated using the NxM input matrix and the given access scheme. The user can now type commands to the console in order to interact with the PRF simulator.

Command         | Description
----------------| -------------
show PRF;                        | Prints  a  graphical  representation  that  shows  how  the  input  matrix  ismapped in the 3D PRF. Each layer highlights in red the elements of theinput matrix stored in the pxq memory modules at the correspondent in-dex.  After the highlighted matrix, a pxq matrix is printed to show whichPRF memory module is storing which data.           
show s;                          | Prints the current schema value.                   
show matrix;                     | Prints the original NxM input matrix.
A[\<num1\>][\<num2\>];               | Performs a single access on the PRF; The memory module storing theaccessed data is identified using the functions m_v() and m_h(), the in-dex within the memory module is computed using A_standard().  Thosesteps  are  all  implemented  in  readFromPR()  which  is  called  when  this command is invoked.                   
A[\<num1\>][\<num2\>],\<ACC_TYPE\>;    | Performs a block access. (num1,num2) are the coordinate of the top-leftelement in the accessed block, the shape of the block is specified by theaccess type \<ACC_TYPE\>.  The original matrix is going to be printed,highlighting the element accessed in parallel. After the output producedby the PRF is shown in the form of a pxq matrix, where each item repre-sent the element read at the respective index.  Lastly the conflict matrixis printed, this shows how many accesses have been performed on eachPRF memory module. Ideally if the block access is conflict free, the ma-trix will contain only 1. The highest number in this matrix also representsthe minimum number of memory accesses necessary, given the currentaccess scheme, to perform the block access.
A*;                              | Performs one block access for each access type, therefore resulting in 5different block accesses.
set s \<num\>;                     | Changes the access scheme used by the PRF. After this command thedata in the PRF are remapped using the given scheme.

