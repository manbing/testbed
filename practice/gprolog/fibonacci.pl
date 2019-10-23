f(0,1).
f(N,F):-
	N>0,
        N1 is N-1,
        f(N1, F1),
        F is F1*N.
