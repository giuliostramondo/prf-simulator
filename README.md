Description                         {#mainpage}
============

The Polymorphic Register File (PRF) are memory modules designed in order to allow fast parallel access to matrices in high performance applications. Through the use of mapping functions ( m_v(int,int,scheme,int,int), m_h(int,int,scheme,int,int), A_standard(int,int,int,int) ) an N-Dimensional matrix is stored in an N+1-Dimensional structure enabling, for certain type of matrix accesses, a faster retrival of the data. The additional dimension in fact should contain N-Dimensional matrices, part of the original matrix, ordered following the sequence of accesses done by the application that is being accelerated. The data contained in the N-Dimensional sub-matrices can then be read in parallel in each computation step.
There are multiple ways in which the sub-matrices can be organized, each one best suited for specific acces pattern:

Access Scheme  | Description
-------------- | -------------
Rectangle only | Generates rectangular patches from the original matrix. 
Rect&Row       | Generates rectangular patches from the original matrix and swaps the rows.
Rect&Col       | Generates rectangular patches from the original matrix and swaps the columns.
Row&Col        | ???
Rect&Trect     | ???

This simulator was written to allow a easier visualization of the effect of each access scheme and to provide a platform for the exploration of custom access schemes.

The simulator is implemented following the description given in ["On implementability of Polymorphic Register Files"].

Installation
============
The sources for the program are available on git ( once obtained the permission from the owner of the repository ). 

To download the code execute the following line from the terminal.

~~~~~~~~~~~~~~~~~~~~~
git clone https://github.com/giuliostramondo/prf-simulator.git
~~~~~~~~~~~~~~~~~~~~~

To compile the sources 
~~~~~~~~~~~~~~~~
cd PRF_Simulator
make
~~~~~~~~~~~~~~~~

This should produce an executable called prf.

Usage
======

Usage: ./prf [Options]


-N <num>	 Change the horizontal size of the input matrix (default 9)

-M <num>	 Change the vertical size of the input matrix (default 9)

-p <num>	 Change the horizontal size of the PRF (default 3)

-q <num>	 Change the horizontal size of the PRF (default 3)

-s <num>	 Change the schema used by the PRF (default 0 -> RECTANGLE_ONLY)
	  other schemes 1->RECT_ROW, 2->RECT_COL, 3->ROW_COL, 4->RECT_TRECT



["On implementability of Polymorphic Register Files"]: http://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=6322873&tag=1 "On implementability of Polymorphic Register Files"
