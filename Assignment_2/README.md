# Advance algorithms Assignment 2

- The Fibonacci sequence can be defined by $T(1) = 1, T(2)$ and
$$T(n) = T(n-1) + T(n-2), n\ge 3$$
Binet proposed a closed formula for the Fibonacci sequence:
$$B(n) = \frac{(\frac{1+\sqrt{5}}{2})^2 - (\frac{1-\sqrt{5}}{2})^n}{\frac{1+\sqrt{5}}{2} - \frac{1-\sqrt{5}}{2}}$$
Show that Binet’s formula is correct, i.e. that $B(n) = T(n) ∀n \ge 1$.

- The Toom-Cook Multiplication Algorithm splits the two input integers $a$ and $b$, both of size $n$, into three parts each
$$a = \beta^{\frac{2n}{3}}a_h + \beta^{\frac{n}{3}}a_m + a_l$$
$$b = \beta^{\frac{2n}{3}}b_h + \beta^{\frac{n}{3}}b_m + b_l$$
combines the six parts $a_h$ through $b_l$ with $O(n)$ operations, obtaining intermediate values $s_1$ through $s_5$ and $r_1$ through $r_5$, each of which has size \frac{n}{3}, executes 5 recursive calls to compute five products $t1 = s1 \times r1$ through $t_5 = s5 \times r5$ and finally combines these five products $t_1$ through $t_5$ in such a way to obtain the complete product $c = a \times b$, using $O(n)$ operations.
