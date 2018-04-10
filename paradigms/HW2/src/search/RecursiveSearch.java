package search;

public class RecursiveSearch {
    //Pre: -1 <= l && r <= array.length && for all i in (l, r): a[i] >= a[i + 1] &&
    //a[l] > x && a[r] <= x

    //Inv:a[l] > x && a[r] <= x
    static int shift(int[] array, int x, int l, int r) {

        if (l + 1 == r) {
            return r;
        }

        int m = (r + l) / 2;

        //l' < m && m < r'
        if (array[m] <= x) {
            return shift(array, x, l, m);
            //a[r] <= x && a[l] > l
        } else {
            return shift(array, x, m, r);
            //a[r] <= x && a[l] > l
        }
        //a[r] <= x && a[l] > l
    }

    //Post:(!I) && a[R - 1] > x && a[R] <= x && for all i in [l, r): a[i] >= a[i + 1] && a[l, r] == a'[l, r]

    public static int search(int[] array, int x) {
        return shift(array, x, -1, array.length);
    }
}
