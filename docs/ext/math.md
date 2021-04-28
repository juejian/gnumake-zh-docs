
测试块数学公式

$$
x^2 + y^2 = z^2\\\\

f(n)=\begin{cases}n/2,&\text{if n  is even}\\3n+1,&\text{if n is odd}\end{cases}
$$


测试行间数学公式

测试 $\sqrt{x^2+y^2} >= 2ab$ 结尾。

$$
x_{ij }^2\quad 
\sqrt{x}\quad 
\sqrt[3]{x}

M = EC^2 \tag{1.1}
$$

$$
\begin{aligned} 
\frac{\partial}{\partial\theta_j}J(\theta) &= \frac{\partial}{\partial\theta_j}\frac{1}{2}(h_\theta(x) - y)^2 \\\\
&= 2\frac{1}{2}(h_\theta(x) - y)\frac{\partial}{\partial\theta_j}(h_\theta(x) - y) \\
&= (h_\theta(x) - y)\frac{\partial}{\partial\theta_j}(\sum_{i=1}^n \theta_ix_i - y) \\
&= (h_\theta(x) - y)x_j
\end{aligned}
$$

$$
\begin{pmatrix}
 1&\alpha_1&\alpha_1^2&\cdots&\alpha_1^n\\
 1&\alpha_2&\alpha_2^2&\cdots&\alpha_2^n\\
 \vdots&\vdots&\vdots&\ddots&\vdots\\
 1&\alpha_n&\alpha_n^2&\cdots&\alpha_n^n
 \end{pmatrix}
 $$

 $$
\begin{pmatrix} a&b\\c&d \end{pmatrix} \quad
\begin{bmatrix} a&b\\c&d \end{bmatrix} \quad
\begin{Bmatrix} a&b\\c&d \end{Bmatrix} \quad
\begin{vmatrix} a&b\\c&d \end{vmatrix} \quad
\begin{Vmatrix} a&b\\c&d \end{Vmatrix}
 $$


$$
\begin{array}{l} \hline
\text{Metropolis-Hastings 采样算法}\\ \hline
1: 初始化马氏链初始状态X _ { 0 } = x _ { 0 }\\
2: 对t = 0,1,2 , \cdots 循环以下过程进行采样\\
\qquad     1: 第t个时刻马氏链的状态为X_{ t } = x_ { t },采样y \sim q (x | x_{ t }) \\
\qquad  2:从均匀分布采样u \sim Uniform[0,1] \\
\qquad  3:如果u < \alpha \left( x _ { t } , y \right) = \min \left\{ \frac { p ( y ) q \left( x _ { t } | y \right) } { p \left( x _ { t } \right) p ( y | x _ { t } ) } , 1 \right\}则接受转移x _ { t } \rightarrow y,即X _ { t + 1 } = y \\
\qquad  4:否则不接受转移,即X _ { t + 1 } = x _ { t } \\ 
\hline \end{array}
$$