##AIMN91
======
This repo is just an expirimental implementation of the dynamic incremental algorithm [AIMN91] for minimal length paths as described in the 'Journal of Algorithms 12 (1991), pp.615-638'.

###some words
__AIMN91__ targets the problem of mainting an All Pairs Shortest Paths Matrix in an on-line fashion. While the algorithm succeeds on solving the puzzle of on line updates on an APSP matrix, which translates to being able to maintain a distance matrix updated without having to reconstruct it every time new edges or new edge weights appear on the problem's graph, It fails badly on providing a viable solution for space sensitive problems, thus failing to keep up with modern techniques and trends where space intensive computations is a major issue (e.g. Big Data applications).

The algorithm showcases some good and hard aspects of dynamic programming, like the use of binary trees and recursive logic to achieve amazingly good time complexity on update operations but it does that by producing and utilizing enormous amount of data which makes it unusable for many real world problem.

The repository cannot be reused in any sense because as it was developed for experimental testing with the algorithm. In case you want to run an execution have in mind that `main.cpp` provides a set of tests and comparisons with other APSP approaches (e.g. Floyd Warshall's APSP) but the whole procedure is not that intuitively layed down.

Have in mind that the code comes with ABSOLUTELY NO WARRANTY.

####prerequisites
You will need C++11 or above with BOOST Library.

####Author
---
Gryllos Prokopis gryllosprokopis@gmail.com
