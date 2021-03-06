
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

/*does gaussian elimination in place in the nxm matrix
 *A[r][c] should be row r, col c
 *returns true if the processes succeeds
 *uses partial pivoting, so reasonably stable with floating point if nessary
 *runtime: O(nm^2)
 *DO NOT use with int, use with either doubles or rationals, rationals suggested
 *the resulting matrix will always be in row-reduced echelon form
 *returns the non-pivot columns
 *the argument k tells the algorithm how many augmented columns there are
 */
template<typename T>
vector<int> gaussian_elimination(T** A,int n, int m, int k,T *pdet){
    vector<int> ret;
    int p_row=0;
    int det=1;
    for(int c=0;c<m;c++){
        //find piviot
        int pivot = -1;
        T p_value = 0;
        for(int r=p_row;r<n;r++){
            if(abs(A[r][c])>p_value){
                p_value = abs(A[r][c]);
                pivot = r;
            }
        }
#ifdef EPSILON
        if(abs(p_value)<EPSILON){
#else
        if(p_value==0){
#endif
            ret.push_back(c);
            if(p_row!=n)
                det = 0;
            continue;
        }
        if(p_row!=pivot){
            swap_ranges(A[p_row],A[p_row]+m+k,A[pivot]);
            det*=((p_row-pivot)%2==0 ? 1 : -1);
        }
        p_value = A[p_row][c];
        for(int r=0;r<n;r++){
            if(r==p_row) continue;
            T mult = A[r][c]/p_value;
            for(int i=c+1;i<m+k;i++)
                A[r][i]-=A[p_row][i]*mult;
            A[r][c]=0;
        }
        A[p_row][c]=1;
        for(int i=c+1;i<m+k;i++) A[p_row][i]/=p_value;
        det*=p_value;
        p_row++;
    }
    if(pdet)
        *pdet=det;
    return ret;
}

//returns a basis for the subspace of homogeneous solutions
//i.e. solutions to Ax=0
template<typename T>
vector<T*> find_homogeneous_solutions(T** A,int n){
    vector<T*> ret;
    vector<int> non_pivots = gaussian_elimination(A,n,n,(double*)0);
    vector<int> pivots;
    int index = 0;
    for(int i=0;i<n;i++){
        if(index>=non_pivots.size()) break;
        if(non_pivots[index]==i) index++;
        else pivots.push_back(i);
    }
    ret.resize(non_pivots.size());
    for(int i=0;i<ret.size();i++){
        ret[i] = new T[n];
        fill_n(ret[i],n,0);
        for(int j=0;j<n-non_pivots.size();j++)
            ret[i][pivots[j]]=-A[non_pivots[i]][j];
        ret[i][non_pivots[i]]=1;
    }
    return ret;
}
