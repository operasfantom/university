#include <cstdio>
#include <cassert>
#include <cstring>

using namespace std;

int next(size_t *p, char *s, size_t n, char c, size_t prev_f) { 
	int j = (prev_f == n ? p[prev_f - 1] : prev_f);
	while (j > 0 && c != s[j]) {
	 	j = p[j - 1];
	}
	if (c == s[j]) {
		j++;
	}
	return j;
}
                                                                                
int main(int argc, char* argv[]) {
	assert(argc == 3);
	freopen(argv[1], "r", stdin);
	size_t n = strlen(argv[2]);
	
	size_t *p = new size_t[n];
	p[0] = 0;
	for (size_t i = 1; i < n; i++) {
	    p[i] = next(p, argv[2], n, argv[2][i], p[i - 1]);
	    //printf("i:%d p[i]:%d\n", i);
	}
	size_t j = 0, i = 0;
	char c;      
	while (scanf("%c", &c) != EOF) {
		j = next(p, argv[2], n, c, j);
		//printf("%c i:%d p[i]:%d\n", c, i, j);
		if (j == n) {
			printf("%d ", i + 1 - n);
		}
		i++;
	}
	return 0;
}
