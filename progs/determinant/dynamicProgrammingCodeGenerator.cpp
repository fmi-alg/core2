
// Produces the CGAL code corresponding to the determinant computation using
// dynamic programming.
//
// This method gives low number of multiplications [ n(2^(n-1)-1) ]
// and additions [ (n-2)2^(n-1)+1 ].  It's nearly optimal for dimension < 6.
// However, this method uses more stack memory (than Gaussian elimination,
// or simple development).  Benchmarks should have the last word, as always.
//
// Author: Sylvain Pion <Sylvain.Pion@sophia.inria.fr>, copyright 1998-9.

#include <stdlib.h>
#include <iostream.h>

void print_current_tab(int rank);
void do_minors (int rank);
bool next_tab (int rank);

int dim;
int *tab; 

int main(char argc, char ** argv)
{
  int i,j,k;

  if (argc != 2)
  {
    cerr << "Usage : " << argv[0] << " dimension" << endl;
    return 1;
  }
  dim = atoi(argv[1]);
  tab = (int *) calloc ( dim, sizeof(int) );
  if (tab == NULL)
  {
    cerr << "Not enough memory." << endl;
    return -1;
  }

  // Prints the headers.
  cout << "FT" << endl << "det" << dim << "x" << dim << "_by_formula (" << endl;
  for (i=0; i<dim; i++)
  {
    for (j=0; j<dim; j++)
      cout << " const FT & a" << i << j << ((j<dim-1) ? ", " : "");
    cout << ((i<dim-1) ? ", " : ")") << endl;
  }
  cout << "{" << endl;

  // Output the minors.

  for (k=2; k<=dim; k++)
  {
    cout << "// Minors of rank " << k << endl;
    do_minors(k);
  }

  // Return value.

  cout << "  return m";
  for (k=0; k<dim; k++)
    cout << k;
  cout << ";" << endl << "}" << endl;

  return 0;
}

void do_minors (int rank)
{
  int i;
  for (i=0; i<rank; i++)
    tab[i] = i;
  tab[dim-1]=dim-1;

  do {
    print_current_tab(rank);
  }
  while (next_tab(rank));
}

bool next_tab (int rank)
{
  if (tab[0] == dim-rank)
    return false;
  
  int i=rank-1;

  if (tab[i]+1 < dim)
  {
    tab[i] ++;
    return true;
  }

  do {
    i--;
  } while (tab[i] == tab[i+1]-1);

  tab[i] ++;

  int j;
  for (j=i+1; j<rank; j++)
    tab[j] = tab[j-1] + 1;

  return true;
}

void print_current_tab (int rank)
{
  int i,j;

  cout << "  FT m";
  for (i=0; i<rank; i++)
    cout << tab[i];
  cout << " = ";

  for (i=rank-1; i>=0; i--)
  {
    if (i < rank-1)
      cout << (((i-rank)%2 != 0) ? " + " : " - ");
    cout << (rank != 2 ? "m" : "a");
    for (j=0; j<rank; j++)
      if (i != j)
        cout << tab[j];
    if (rank == 2)
      cout << "0";

    cout << "*a" << tab[i] << rank-1;
  }
  cout << ";" << endl;
}
