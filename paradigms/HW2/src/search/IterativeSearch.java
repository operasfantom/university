package search;

public class IterativeSearch {

    public static int search(int[] array, int x){
        //for all i in [-1, a.length): a[i] >= a[i + 1]

        int l = -1, r = array.length;

        //l = -1 && r = a.length >= 0, let's consider that a[-1] = INF && a[a.length] == -INF
        //Inv:a[l] > x && a[r] <= x && -1 <= l < r - 1 < a.length
        while (r - l > 1) {
            int m = (r + l) / 2;
            //Pre: l < m && m < r
            if (array[m] < x) {
                //array[m] < x
                r = m;
                //array[r] < x
                //r < r'
            } else {
                //array[m] >= x
                l = m;
                //array[l] >= x
                //l > l'
            }
            //Post: [new_l, new_r] strongly includes in [l, r]
        }
        //(!Inv && l + 1 == r) => (R = r :: a[R - 1] > x && a[R] <= x)
        return r;
    }
}
