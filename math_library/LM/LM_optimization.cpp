/*************************************************************************
                IMPROVED LEVENBERG-MARQUARDT METHOD FOR
                 NON-LINEAR LEAST SQUARES OPTIMIZATION

DESCRIPTION:
This function is used to find minimum of function which is represented  as
sum of squares:
    F(x) = f[0]^2(x[0],...,x[n-1]) + ... + f[m-1]^2(x[0],...,x[n-1])
using value of function vector f[] only. Finite differences  are  used  to
calculate Jacobian.


REQUIREMENTS:
This algorithm will request following information during its operation:
* function vector f[] at given point X

There are several overloaded versions of  MinLMOptimize()  function  which
correspond  to  different LM-like optimization algorithms provided by this
unit. You should choose version which accepts fvec() callback.

You can try to initialize MinLMState structure with VJ  function and  then
use incorrect version  of  MinLMOptimize()  (for  example,  version  which
works with general form function and does not accept function vector), but
it will  lead  to  exception being thrown after first attempt to calculate
Jacobian.


USAGE:
1. User initializes algorithm state with MinLMCreateV() call
2. User tunes solver parameters with MinLMSetCond(),  MinLMSetStpMax() and
   other functions
3. User calls MinLMOptimize() function which  takes algorithm  state   and
   callback functions.
4. User calls MinLMResults() to get solution
5. Optionally, user may call MinLMRestartFrom() to solve  another  problem
   with same N/M but another starting point and/or another function.
   MinLMRestartFrom() allows to reuse already initialized structure.


INPUT PARAMETERS:
    N       -   dimension, N>1
                * if given, only leading N elements of X are used
                * if not given, automatically determined from size of X
    M       -   number of functions f[i]
    X       -   initial solution, array[0..N-1]
    DiffStep-   differentiation step, >0

OUTPUT PARAMETERS:
    State   -   structure which stores algorithm state

See also MinLMIteration, MinLMResults.

NOTES:
1. you may tune stopping conditions with MinLMSetCond() function
2. if target function contains exp() or other fast growing functions,  and
   optimization algorithm makes too large steps which leads  to  overflow,
   use MinLMSetStpMax() function to bound algorithm's steps.

  -- ALGLIB --
     Copyright 30.03.2009 by Bochkanov Sergey
*************************************************************************/
void minlmcreatev(const ae_int_t n, const ae_int_t m, const real_1d_array &x, const double diffstep, minlmstate &state, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmcreatev(n, m, x.c_ptr(), diffstep, state.c_ptr(), &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
                IMPROVED LEVENBERG-MARQUARDT METHOD FOR
                 NON-LINEAR LEAST SQUARES OPTIMIZATION

DESCRIPTION:
This function is used to find minimum of function which is represented  as
sum of squares:
    F(x) = f[0]^2(x[0],...,x[n-1]) + ... + f[m-1]^2(x[0],...,x[n-1])
using value of function vector f[] only. Finite differences  are  used  to
calculate Jacobian.


REQUIREMENTS:
This algorithm will request following information during its operation:
* function vector f[] at given point X

There are several overloaded versions of  MinLMOptimize()  function  which
correspond  to  different LM-like optimization algorithms provided by this
unit. You should choose version which accepts fvec() callback.

You can try to initialize MinLMState structure with VJ  function and  then
use incorrect version  of  MinLMOptimize()  (for  example,  version  which
works with general form function and does not accept function vector), but
it will  lead  to  exception being thrown after first attempt to calculate
Jacobian.


USAGE:
1. User initializes algorithm state with MinLMCreateV() call
2. User tunes solver parameters with MinLMSetCond(),  MinLMSetStpMax() and
   other functions
3. User calls MinLMOptimize() function which  takes algorithm  state   and
   callback functions.
4. User calls MinLMResults() to get solution
5. Optionally, user may call MinLMRestartFrom() to solve  another  problem
   with same N/M but another starting point and/or another function.
   MinLMRestartFrom() allows to reuse already initialized structure.


INPUT PARAMETERS:
    N       -   dimension, N>1
                * if given, only leading N elements of X are used
                * if not given, automatically determined from size of X
    M       -   number of functions f[i]
    X       -   initial solution, array[0..N-1]
    DiffStep-   differentiation step, >0

OUTPUT PARAMETERS:
    State   -   structure which stores algorithm state

See also MinLMIteration, MinLMResults.

NOTES:
1. you may tune stopping conditions with MinLMSetCond() function
2. if target function contains exp() or other fast growing functions,  and
   optimization algorithm makes too large steps which leads  to  overflow,
   use MinLMSetStpMax() function to bound algorithm's steps.

  -- ALGLIB --
     Copyright 30.03.2009 by Bochkanov Sergey
*************************************************************************/
#if !defined(AE_NO_EXCEPTIONS)
void minlmcreatev(const ae_int_t m, const real_1d_array &x, const double diffstep, minlmstate &state, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;    
    ae_int_t n;

    n = x.length();
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmcreatev(n, m, x.c_ptr(), diffstep, state.c_ptr(), &_alglib_env_state);

    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}
#endif

/*************************************************************************
This function sets stopping conditions for Levenberg-Marquardt optimization
algorithm.

INPUT PARAMETERS:
    State   -   structure which stores algorithm state
    EpsX    -   >=0
                The subroutine finishes its work if  on  k+1-th  iteration
                the condition |v|<=EpsX is fulfilled, where:
                * |.| means Euclidian norm
                * v - scaled step vector, v[i]=dx[i]/s[i]
                * dx - ste pvector, dx=X(k+1)-X(k)
                * s - scaling coefficients set by MinLMSetScale()
                Recommended values: 1E-9 ... 1E-12.
    MaxIts  -   maximum number of iterations. If MaxIts=0, the  number  of
                iterations   is    unlimited.   Only   Levenberg-Marquardt
                iterations  are  counted  (L-BFGS/CG  iterations  are  NOT
                counted because their cost is very low compared to that of
                LM).

Passing  EpsX=0  and  MaxIts=0  (simultaneously)  will  lead  to automatic
stopping criterion selection (small EpsX).

NOTE: it is not recommended to set large EpsX (say, 0.001). Because LM  is
      a second-order method, it performs very precise steps anyway.

  -- ALGLIB --
     Copyright 02.04.2010 by Bochkanov Sergey
*************************************************************************/
void minlmsetcond(minlmstate &state, const double epsx, const ae_int_t maxits, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmsetcond(state.c_ptr(), epsx, maxits, &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This function turns on/off reporting.

INPUT PARAMETERS:
    State   -   structure which stores algorithm state
    NeedXRep-   whether iteration reports are needed or not

If NeedXRep is True, algorithm will call rep() callback function if  it is
provided to MinLMOptimize(). Both Levenberg-Marquardt and internal  L-BFGS
iterations are reported.

  -- ALGLIB --
     Copyright 02.04.2010 by Bochkanov Sergey
*************************************************************************/
void minlmsetxrep(minlmstate &state, const bool needxrep, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmsetxrep(state.c_ptr(), needxrep, &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This function sets maximum step length

INPUT PARAMETERS:
    State   -   structure which stores algorithm state
    StpMax  -   maximum step length, >=0. Set StpMax to 0.0,  if you don't
                want to limit step length.

Use this subroutine when you optimize target function which contains exp()
or  other  fast  growing  functions,  and optimization algorithm makes too
large  steps  which  leads  to overflow. This function allows us to reject
steps  that  are  too  large  (and  therefore  expose  us  to the possible
overflow) without actually calculating function value at the x+stp*d.

NOTE: non-zero StpMax leads to moderate  performance  degradation  because
intermediate  step  of  preconditioned L-BFGS optimization is incompatible
with limits on step size.

  -- ALGLIB --
     Copyright 02.04.2010 by Bochkanov Sergey
*************************************************************************/
void minlmsetstpmax(minlmstate &state, const double stpmax, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmsetstpmax(state.c_ptr(), stpmax, &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This function sets scaling coefficients for LM optimizer.

ALGLIB optimizers use scaling matrices to test stopping  conditions  (step
size and gradient are scaled before comparison with tolerances).  Scale of
the I-th variable is a translation invariant measure of:
a) "how large" the variable is
b) how large the step should be to make significant changes in the function

Generally, scale is NOT considered to be a form of preconditioner.  But LM
optimizer is unique in that it uses scaling matrix both  in  the  stopping
condition tests and as Marquardt damping factor.

Proper scaling is very important for the algorithm performance. It is less
important for the quality of results, but still has some influence (it  is
easier  to  converge  when  variables  are  properly  scaled, so premature
stopping is possible when very badly scalled variables are  combined  with
relaxed stopping conditions).

INPUT PARAMETERS:
    State   -   structure stores algorithm state
    S       -   array[N], non-zero scaling coefficients
                S[i] may be negative, sign doesn't matter.

  -- ALGLIB --
     Copyright 14.01.2011 by Bochkanov Sergey
*************************************************************************/
void minlmsetscale(minlmstate &state, const real_1d_array &s, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmsetscale(state.c_ptr(), s.c_ptr(), &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This function sets boundary constraints for LM optimizer

Boundary constraints are inactive by default (after initial creation).
They are preserved until explicitly turned off with another SetBC() call.

INPUT PARAMETERS:
    State   -   structure stores algorithm state
    BndL    -   lower bounds, array[N].
                If some (all) variables are unbounded, you may specify
                very small number or -INF (latter is recommended because
                it will allow solver to use better algorithm).
    BndU    -   upper bounds, array[N].
                If some (all) variables are unbounded, you may specify
                very large number or +INF (latter is recommended because
                it will allow solver to use better algorithm).

NOTE 1: it is possible to specify BndL[i]=BndU[i]. In this case I-th
variable will be "frozen" at X[i]=BndL[i]=BndU[i].

NOTE 2: this solver has following useful properties:
* bound constraints are always satisfied exactly
* function is evaluated only INSIDE area specified by bound constraints
  or at its boundary

  -- ALGLIB --
     Copyright 14.01.2011 by Bochkanov Sergey
*************************************************************************/
void minlmsetbc(minlmstate &state, const real_1d_array &bndl, const real_1d_array &bndu, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmsetbc(state.c_ptr(), bndl.c_ptr(), bndu.c_ptr(), &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This function sets general linear constraints for LM optimizer

Linear constraints are inactive by default (after initial creation).  They
are preserved until explicitly turned off with another minlmsetlc() call.

INPUT PARAMETERS:
    State   -   structure stores algorithm state
    C       -   linear constraints, array[K,N+1].
                Each row of C represents one constraint, either equality
                or inequality (see below):
                * first N elements correspond to coefficients,
                * last element corresponds to the right part.
                All elements of C (including right part) must be finite.
    CT      -   type of constraints, array[K]:
                * if CT[i]>0, then I-th constraint is C[i,*]*x >= C[i,n+1]
                * if CT[i]=0, then I-th constraint is C[i,*]*x  = C[i,n+1]
                * if CT[i]<0, then I-th constraint is C[i,*]*x <= C[i,n+1]
    K       -   number of equality/inequality constraints, K>=0:
                * if given, only leading K elements of C/CT are used
                * if not given, automatically determined from sizes of C/CT

IMPORTANT: if you have linear constraints, it is strongly  recommended  to
           set scale of variables with minlmsetscale(). QP solver which is
           used to calculate linearly constrained steps heavily relies  on
           good scaling of input problems.

IMPORTANT: solvers created with minlmcreatefgh()  do  not  support  linear
           constraints.

NOTE: linear  (non-bound)  constraints are satisfied only approximately  -
      there  always  exists some violation due  to  numerical  errors  and
      algorithmic limitations.

NOTE: general linear constraints  add  significant  overhead  to  solution
      process. Although solver performs roughly same amount of  iterations
      (when compared  with  similar  box-only  constrained  problem), each
      iteration   now    involves  solution  of  linearly  constrained  QP
      subproblem, which requires ~3-5 times more Cholesky  decompositions.
      Thus, if you can reformulate your problem in such way  this  it  has
      only box constraints, it may be beneficial to do so.

  -- ALGLIB --
     Copyright 14.01.2011 by Bochkanov Sergey
*************************************************************************/
void minlmsetlc(minlmstate &state, const real_2d_array &c, const integer_1d_array &ct, const ae_int_t k, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmsetlc(state.c_ptr(), c.c_ptr(), ct.c_ptr(), k, &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This function sets general linear constraints for LM optimizer

Linear constraints are inactive by default (after initial creation).  They
are preserved until explicitly turned off with another minlmsetlc() call.

INPUT PARAMETERS:
    State   -   structure stores algorithm state
    C       -   linear constraints, array[K,N+1].
                Each row of C represents one constraint, either equality
                or inequality (see below):
                * first N elements correspond to coefficients,
                * last element corresponds to the right part.
                All elements of C (including right part) must be finite.
    CT      -   type of constraints, array[K]:
                * if CT[i]>0, then I-th constraint is C[i,*]*x >= C[i,n+1]
                * if CT[i]=0, then I-th constraint is C[i,*]*x  = C[i,n+1]
                * if CT[i]<0, then I-th constraint is C[i,*]*x <= C[i,n+1]
    K       -   number of equality/inequality constraints, K>=0:
                * if given, only leading K elements of C/CT are used
                * if not given, automatically determined from sizes of C/CT

IMPORTANT: if you have linear constraints, it is strongly  recommended  to
           set scale of variables with minlmsetscale(). QP solver which is
           used to calculate linearly constrained steps heavily relies  on
           good scaling of input problems.

IMPORTANT: solvers created with minlmcreatefgh()  do  not  support  linear
           constraints.

NOTE: linear  (non-bound)  constraints are satisfied only approximately  -
      there  always  exists some violation due  to  numerical  errors  and
      algorithmic limitations.

NOTE: general linear constraints  add  significant  overhead  to  solution
      process. Although solver performs roughly same amount of  iterations
      (when compared  with  similar  box-only  constrained  problem), each
      iteration   now    involves  solution  of  linearly  constrained  QP
      subproblem, which requires ~3-5 times more Cholesky  decompositions.
      Thus, if you can reformulate your problem in such way  this  it  has
      only box constraints, it may be beneficial to do so.

  -- ALGLIB --
     Copyright 14.01.2011 by Bochkanov Sergey
*************************************************************************/
#if !defined(AE_NO_EXCEPTIONS)
void minlmsetlc(minlmstate &state, const real_2d_array &c, const integer_1d_array &ct, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;    
    ae_int_t k;
    if( (c.rows()!=ct.length()))
        _ALGLIB_CPP_EXCEPTION("Error while calling 'minlmsetlc': looks like one of arguments has wrong size");
    k = c.rows();
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmsetlc(state.c_ptr(), c.c_ptr(), ct.c_ptr(), k, &_alglib_env_state);

    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}
#endif

/*************************************************************************
This function is used to change acceleration settings

You can choose between three acceleration strategies:
* AccType=0, no acceleration.
* AccType=1, secant updates are used to update quadratic model after  each
  iteration. After fixed number of iterations (or after  model  breakdown)
  we  recalculate  quadratic  model  using  analytic  Jacobian  or  finite
  differences. Number of secant-based iterations depends  on  optimization
  settings: about 3 iterations - when we have analytic Jacobian, up to 2*N
  iterations - when we use finite differences to calculate Jacobian.

AccType=1 is recommended when Jacobian  calculation  cost is prohibitively
high (several Mx1 function vector calculations  followed  by  several  NxN
Cholesky factorizations are faster than calculation of one M*N  Jacobian).
It should also be used when we have no Jacobian, because finite difference
approximation takes too much time to compute.

Table below list  optimization  protocols  (XYZ  protocol  corresponds  to
MinLMCreateXYZ) and acceleration types they support (and use by  default).

ACCELERATION TYPES SUPPORTED BY OPTIMIZATION PROTOCOLS:

protocol    0   1   comment
V           +   +
VJ          +   +
FGH         +

DEFAULT VALUES:

protocol    0   1   comment
V               x   without acceleration it is so slooooooooow
VJ          x
FGH         x

NOTE: this  function should be called before optimization. Attempt to call
it during algorithm iterations may result in unexpected behavior.

NOTE: attempt to call this function with unsupported protocol/acceleration
combination will result in exception being thrown.

  -- ALGLIB --
     Copyright 14.10.2010 by Bochkanov Sergey
*************************************************************************/
void minlmsetacctype(minlmstate &state, const ae_int_t acctype, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmsetacctype(state.c_ptr(), acctype, &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This function is used  to  activate/deactivate  nonmonotonic  steps.  Such
steps may improve  convergence  on  noisy  problems  or  ones  with  minor
smoothness defects.

In its standard mode, LM solver compares value at the trial   point   f[1]
with the value at the current point f[0]. Only steps that decrease f() are
accepted.

When the nonmonotonic mode is activated, f[1]  is  compared  with  maximum
over several previous  locations:  max(f[0],f[-1],...,f[-CNT]).  We  still
accept only steps that decrease  f(),  however  our  reference  value  has
changed. The net results is that f[1]>f[0] are now allowed.

Nonmonotonic steps can help to handle minor defects in the objective (e.g.
small  noise,  discontinuous  jumps  or  nonsmoothness).  However,  it  is
important  that  the  overall  shape  of  the  problem  is  still  smooth.
It  may  also  help  to  minimize  perfectly  smooth  targets with complex
geometries by allowing to jump through curved valleys.

However, sometimes nonmonotonic steps degrade convergence by  allowing  an
optimizer to wander too far away from the solution, so this feature should
be used only after careful testing.

INPUT PARAMETERS:
    State   -   structure stores algorithm state
    Cnt     -   nonmonotonic memory length, Cnt>=0:
                * 0 for traditional monotonic steps
                * 2..3 is recommended for the nonmonotonic optimization

  -- ALGLIB --
     Copyright 07.04.2024 by Bochkanov Sergey
*************************************************************************/
void minlmsetnonmonotonicsteps(minlmstate &state, const ae_int_t cnt, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmsetnonmonotonicsteps(state.c_ptr(), cnt, &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This function provides reverse communication interface
Reverse communication interface is not documented or recommended to use.
See below for functions which provide better documented API
*************************************************************************/
bool minlmiteration(minlmstate &state, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return 0;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    ae_bool result = alglib_impl::minlmiteration(state.c_ptr(), &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return bool(result);
}


void minlmoptimize(minlmstate &state,
    void (*fvec)(const real_1d_array &x, real_1d_array &fi, void *ptr),
    void  (*rep)(const real_1d_array &x, double func, void *ptr),
    void *ptr,
    const xparams _xparams)
{
    alglib_impl::minlmstate &optimizer = *(state.c_ptr());
    alglib_impl::rcommv2_request request(
        optimizer.requesttype,
        optimizer.querysize, optimizer.queryfuncs, optimizer.queryvars, optimizer.querydim, optimizer.queryformulasize,
        optimizer.querydata.ptr.p_double, optimizer.replyfi.ptr.p_double, optimizer.replydj.ptr.p_double, optimizer.replysj, ptr,
        "minlm");
    alglib_impl::rcommv2_callbacks callbacks;
    alglib_impl::rcommv2_buffers buffers(&state.c_ptr()->tmpx1, &state.c_ptr()->tmpc1, &state.c_ptr()->tmpf1, &state.c_ptr()->tmpg1, &state.c_ptr()->tmpj1, &state.c_ptr()->tmps1);
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::ae_assert(fvec!=NULL, "ALGLIB: error in 'minlmoptimize()' (fvec is NULL)", &_alglib_env_state);
callbacks.fvec = fvec;

    alglib_impl::minlmsetprotocolv2(state.c_ptr(), &_alglib_env_state);
    while( alglib_impl::minlmiteration(state.c_ptr(), &_alglib_env_state) )
    {
        _ALGLIB_CALLBACK_EXCEPTION_GUARD_BEGIN
        if( optimizer.requesttype==3 )
        {
            const ae_int_t njobs = request.size*request.vars+request.size;
            for(alglib_impl::ae_int_t job_idx=0; job_idx<njobs; job_idx++)
                alglib_impl::process_v2request_3phase0(request, job_idx, callbacks, buffers);
            alglib_impl::process_v2request_3phase1(request);
            request.request = 0;
            continue;
        }
        if( optimizer.requesttype==5 )
        {
            const ae_int_t njobs = request.size*request.vars+request.size;
            for(alglib_impl::ae_int_t job_idx=0; job_idx<njobs; job_idx++)
                alglib_impl::process_v2request_5phase0(request, job_idx, callbacks, buffers);
            alglib_impl::process_v2request_5phase1(request);
            request.request = 0;
            continue;
        }if( optimizer.requesttype==4 )
        { 
            for(alglib_impl::ae_int_t qidx=0; qidx<optimizer.querysize; qidx++)
                alglib_impl::process_v2request_4(request, qidx, callbacks, buffers);
            optimizer.requesttype = 0;
            continue;
        }
        if( optimizer.requesttype==-1 )
        {
            memmove(&buffers.tmpX[0], optimizer.reportx.ptr.p_double, optimizer.queryvars*sizeof(double));
            if( rep!=NULL )
                rep(buffers.tmpX, optimizer.reportf, ptr);
            continue;
        }
        goto lbl_no_callback;
        _ALGLIB_CALLBACK_EXCEPTION_GUARD_END
    }
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
lbl_no_callback:
    alglib_impl::ae_assert(ae_false, "ALGLIB: error in 'minlmoptimize' (some derivatives were not provided?)", &_alglib_env_state);
}


void minlmoptimize(minlmstate &state,
    void (*fvec)(const real_1d_array &x, real_1d_array &fi, void *ptr),
    void  (*jac)(const real_1d_array &x, real_1d_array &fi, real_2d_array &jac, void *ptr),
    void  (*rep)(const real_1d_array &x, double func, void *ptr),
    void *ptr,
    const xparams _xparams)
{
    alglib_impl::minlmstate &optimizer = *(state.c_ptr());
    alglib_impl::rcommv2_request request(
        optimizer.requesttype,
        optimizer.querysize, optimizer.queryfuncs, optimizer.queryvars, optimizer.querydim, optimizer.queryformulasize,
        optimizer.querydata.ptr.p_double, optimizer.replyfi.ptr.p_double, optimizer.replydj.ptr.p_double, optimizer.replysj, ptr,
        "minlm");
    alglib_impl::rcommv2_callbacks callbacks;
    alglib_impl::rcommv2_buffers buffers(&state.c_ptr()->tmpx1, &state.c_ptr()->tmpc1, &state.c_ptr()->tmpf1, &state.c_ptr()->tmpg1, &state.c_ptr()->tmpj1, &state.c_ptr()->tmps1);
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::ae_assert(fvec!=NULL, "ALGLIB: error in 'minlmoptimize()' (fvec is NULL)", &_alglib_env_state);
    alglib_impl::ae_assert(jac!=NULL,  "ALGLIB: error in 'minlmoptimize()' (jac is NULL)", &_alglib_env_state);
callbacks.fvec = fvec;
callbacks.jac = jac;

    alglib_impl::minlmsetprotocolv2(state.c_ptr(), &_alglib_env_state);
    while( alglib_impl::minlmiteration(state.c_ptr(), &_alglib_env_state) )
    {
        _ALGLIB_CALLBACK_EXCEPTION_GUARD_BEGIN
        if( optimizer.requesttype==2 )
        { 
            for(alglib_impl::ae_int_t qidx=0; qidx<optimizer.querysize; qidx++)
                alglib_impl::process_v2request_2(request, qidx, callbacks, buffers);
            optimizer.requesttype = 0;
            continue;
        }if( optimizer.requesttype==4 )
        { 
            for(alglib_impl::ae_int_t qidx=0; qidx<optimizer.querysize; qidx++)
                alglib_impl::process_v2request_4(request, qidx, callbacks, buffers);
            optimizer.requesttype = 0;
            continue;
        }
        if( optimizer.requesttype==-1 )
        {
            memmove(&buffers.tmpX[0], optimizer.reportx.ptr.p_double, optimizer.queryvars*sizeof(double));
            if( rep!=NULL )
                rep(buffers.tmpX, optimizer.reportf, ptr);
            continue;
        }
        goto lbl_no_callback;
        _ALGLIB_CALLBACK_EXCEPTION_GUARD_END
    }
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
lbl_no_callback:
    alglib_impl::ae_assert(ae_false, "ALGLIB: error in 'minlmoptimize' (some derivatives were not provided?)", &_alglib_env_state);
}



/*************************************************************************
This  function  activates/deactivates verification  of  the  user-supplied
analytic Jacobian.

Upon  activation  of  this  option  OptGuard  integrity  checker  performs
numerical differentiation of your target function vector  at  the  initial
point (note: future versions may also perform check  at  the final  point)
and compares numerical Jacobian with analytic one provided by you.

If difference is too large, an error flag is set and optimization  session
continues. After optimization session is over, you can retrieve the report
which stores  both  Jacobians,  and  specific  components  highlighted  as
suspicious by the OptGuard.

The OptGuard report can be retrieved with minlmoptguardresults().

IMPORTANT: gradient check is a high-overhead option which  will  cost  you
           about 3*N additional function evaluations. In many cases it may
           cost as much as the rest of the optimization session.

           YOU SHOULD NOT USE IT IN THE PRODUCTION CODE UNLESS YOU WANT TO
           CHECK DERIVATIVES PROVIDED BY SOME THIRD PARTY.

NOTE: unlike previous incarnation of the gradient checking code,  OptGuard
      does NOT interrupt optimization even if it discovers bad gradient.

INPUT PARAMETERS:
    State       -   structure used to store algorithm state
    TestStep    -   verification step used for numerical differentiation:
                    * TestStep=0 turns verification off
                    * TestStep>0 activates verification
                    You should carefully choose TestStep. Value  which  is
                    too large (so large that  function  behavior  is  non-
                    cubic at this scale) will lead  to  false  alarms. Too
                    short step will result in rounding  errors  dominating
                    numerical derivative.

                    You may use different step for different parameters by
                    means of setting scale with minlmsetscale().

=== EXPLANATION ==========================================================

In order to verify gradient algorithm performs following steps:
  * two trial steps are made to X[i]-TestStep*S[i] and X[i]+TestStep*S[i],
    where X[i] is i-th component of the initial point and S[i] is a  scale
    of i-th parameter
  * F(X) is evaluated at these trial points
  * we perform one more evaluation in the middle point of the interval
  * we  build  cubic  model using function values and derivatives at trial
    points and we compare its prediction with actual value in  the  middle
    point

  -- ALGLIB --
     Copyright 15.06.2014 by Bochkanov Sergey
*************************************************************************/
void minlmoptguardgradient(minlmstate &state, const double teststep, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmoptguardgradient(state.c_ptr(), teststep, &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
Results of OptGuard integrity check, should be called  after  optimization
session is over.

OptGuard checks analytic Jacobian  against  reference  value  obtained  by
numerical differentiation with user-specified step.

NOTE: other optimizers perform additional OptGuard checks for things  like
      C0/C1-continuity violations. However, LM optimizer  can  check  only
      for incorrect Jacobian.

      The reason is that unlike line search methods LM optimizer does  not
      perform extensive evaluations along the line. Thus, we simply do not
      have enough data to catch C0/C1-violations.

This check is activated with  minlmoptguardgradient() function.

Following flags are set when these errors are suspected:
* rep.badgradsuspected, and additionally:
  * rep.badgradfidx for specific function (Jacobian row) suspected
  * rep.badgradvidx for specific variable (Jacobian column) suspected
  * rep.badgradxbase, a point where gradient/Jacobian is tested
  * rep.badgraduser, user-provided gradient/Jacobian
  * rep.badgradnum, reference gradient/Jacobian obtained via numerical
    differentiation

INPUT PARAMETERS:
    state   -   algorithm state

OUTPUT PARAMETERS:
    rep     -   OptGuard report

  -- ALGLIB --
     Copyright 21.11.2018 by Bochkanov Sergey
*************************************************************************/
void minlmoptguardresults(minlmstate &state, optguardreport &rep, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmoptguardresults(state.c_ptr(), rep.c_ptr(), &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
Levenberg-Marquardt algorithm results

NOTE: if you activated OptGuard integrity checking functionality and  want
      to get OptGuard report,  it  can  be  retrieved  with  the  help  of
      minlmoptguardresults() function.

INPUT PARAMETERS:
    State   -   algorithm state

OUTPUT PARAMETERS:
    X       -   array[0..N-1], solution
    Rep     -   optimization  report;  includes  termination   codes   and
                additional information. Termination codes are listed below,
                see comments for this structure for more info.
                Termination code is stored in rep.terminationtype field:
                * -8    optimizer detected NAN/INF values either in the
                        function itself, or in its Jacobian
                * -3    constraints are inconsistent
                *  2    relative step is no more than EpsX.
                *  5    MaxIts steps was taken
                *  7    stopping conditions are too stringent,
                        further improvement is impossible
                *  8    terminated by user who called minlmrequesttermination().
                        X contains point which was "current accepted" when
                        termination request was submitted.

  -- ALGLIB --
     Copyright 10.03.2009 by Bochkanov Sergey
*************************************************************************/
void minlmresults(const minlmstate &state, real_1d_array &x, minlmreport &rep, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmresults(state.c_ptr(), x.c_ptr(), rep.c_ptr(), &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
Levenberg-Marquardt algorithm results

Buffered implementation of MinLMResults(), which uses pre-allocated buffer
to store X[]. If buffer size is  too  small,  it  resizes  buffer.  It  is
intended to be used in the inner cycles of performance critical algorithms
where array reallocation penalty is too large to be ignored.

  -- ALGLIB --
     Copyright 10.03.2009 by Bochkanov Sergey
*************************************************************************/
void minlmresultsbuf(const minlmstate &state, real_1d_array &x, minlmreport &rep, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmresultsbuf(state.c_ptr(), x.c_ptr(), rep.c_ptr(), &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This  subroutine  restarts  LM  algorithm from new point. All optimization
parameters are left unchanged.

This  function  allows  to  solve multiple  optimization  problems  (which
must have same number of dimensions) without object reallocation penalty.

INPUT PARAMETERS:
    State   -   structure used for reverse communication previously
                allocated with MinLMCreateXXX call.
    X       -   new starting point.

  -- ALGLIB --
     Copyright 30.07.2010 by Bochkanov Sergey
*************************************************************************/
void minlmrestartfrom(minlmstate &state, const real_1d_array &x, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmrestartfrom(state.c_ptr(), x.c_ptr(), &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}

/*************************************************************************
This subroutine submits request for termination of running  optimizer.  It
should be called from user-supplied callback when user decides that it  is
time to "smoothly" terminate optimization process.  As  result,  optimizer
stops at point which was "current accepted" when termination  request  was
submitted and returns error code 8 (successful termination).

INPUT PARAMETERS:
    State   -   optimizer structure

NOTE: after  request  for  termination  optimizer  may   perform   several
      additional calls to user-supplied callbacks. It does  NOT  guarantee
      to stop immediately - it just guarantees that these additional calls
      will be discarded later.

NOTE: calling this function on optimizer which is NOT running will have no
      effect.

NOTE: multiple calls to this function are possible. First call is counted,
      subsequent calls are silently ignored.

  -- ALGLIB --
     Copyright 08.10.2014 by Bochkanov Sergey
*************************************************************************/
void minlmrequesttermination(minlmstate &state, const xparams _xparams)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl::ae_state_init(&_alglib_env_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_alglib_env_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_alglib_env_state.error_msg);
        return;
#endif
    }
    ae_state_set_break_jump(&_alglib_env_state, &_break_jump);
    if( _xparams.flags!=(alglib_impl::ae_uint64_t)0x0 )
        ae_state_set_flags(&_alglib_env_state, _xparams.flags);
    alglib_impl::minlmrequesttermination(state.c_ptr(), &_alglib_env_state);
    alglib_impl::ae_state_clear(&_alglib_env_state);
    return;
}
#endif

#if defined(AE_COMPILE_NLCAUL) || !defined(AE_PARTIAL_BUILD)

#endif

#if defined(AE_COMPILE_MINCG) || !defined(AE_PARTIAL_BUILD)
/*************************************************************************
This object stores state of the nonlinear CG optimizer.

You should use ALGLIB functions to work with this object.
*************************************************************************/
_mincgstate_owner::_mincgstate_owner()
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _state;
    
    alglib_impl::ae_state_init(&_state);
    if( setjmp(_break_jump) )
    {
        if( p_struct!=NULL )
        {
            alglib_impl::_mincgstate_destroy(p_struct);
            alglib_impl::ae_free(p_struct);
        }
        p_struct = NULL;
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_state.error_msg);
        return;
#endif
    }
    alglib_impl::ae_state_set_break_jump(&_state, &_break_jump);
    p_struct = NULL;
    p_struct = (alglib_impl::mincgstate*)alglib_impl::ae_malloc(sizeof(alglib_impl::mincgstate), &_state);
    memset(p_struct, 0, sizeof(alglib_impl::mincgstate));
    alglib_impl::_mincgstate_init(p_struct, &_state, ae_false);
    ae_state_clear(&_state);
    is_attached = false;
}

_mincgstate_owner::_mincgstate_owner(alglib_impl::mincgstate *attach_to)
{
    p_struct = attach_to;
    is_attached = true;
}

_mincgstate_owner::_mincgstate_owner(const _mincgstate_owner &rhs)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _state;
    
    alglib_impl::ae_state_init(&_state);
    if( setjmp(_break_jump) )
    {
        if( p_struct!=NULL )
        {
            alglib_impl::_mincgstate_destroy(p_struct);
            alglib_impl::ae_free(p_struct);
        }
        p_struct = NULL;
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_state.error_msg);
        return;
#endif
    }
    alglib_impl::ae_state_set_break_jump(&_state, &_break_jump);
    p_struct = NULL;
    alglib_impl::ae_assert(rhs.p_struct!=NULL, "ALGLIB: mincgstate copy constructor failure (source is not initialized)", &_state);
    p_struct = (alglib_impl::mincgstate*)alglib_impl::ae_malloc(sizeof(alglib_impl::mincgstate), &_state);
    memset(p_struct, 0, sizeof(alglib_impl::mincgstate));
    alglib_impl::_mincgstate_init_copy(p_struct, const_cast<alglib_impl::mincgstate*>(rhs.p_struct), &_state, ae_false);
    ae_state_clear(&_state);
    is_attached = false;
}

_mincgstate_owner& _mincgstate_owner::operator=(const _mincgstate_owner &rhs)
{
    if( this==&rhs )
        return *this;
    jmp_buf _break_jump;
    alglib_impl::ae_state _state;
    
    alglib_impl::ae_state_init(&_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_state.error_msg);
        return *this;
#endif
    }
    alglib_impl::ae_state_set_break_jump(&_state, &_break_jump);
    alglib_impl::ae_assert(p_struct!=NULL, "ALGLIB: mincgstate assignment constructor failure (destination is not initialized)", &_state);
    alglib_impl::ae_assert(rhs.p_struct!=NULL, "ALGLIB: mincgstate assignment constructor failure (source is not initialized)", &_state);
    alglib_impl::ae_assert(!is_attached, "ALGLIB: mincgstate assignment constructor failure (can not assign to the structure which is attached to something else)", &_state);
    alglib_impl::_mincgstate_destroy(p_struct);
    memset(p_struct, 0, sizeof(alglib_impl::mincgstate));
    alglib_impl::_mincgstate_init_copy(p_struct, const_cast<alglib_impl::mincgstate*>(rhs.p_struct), &_state, ae_false);
    ae_state_clear(&_state);
    return *this;
}

_mincgstate_owner::~_mincgstate_owner()
{
    if( p_struct!=NULL && !is_attached )
    {
        alglib_impl::_mincgstate_destroy(p_struct);
        ae_free(p_struct);
    }
}

alglib_impl::mincgstate* _mincgstate_owner::c_ptr()
{
    return p_struct;
}

const alglib_impl::mincgstate* _mincgstate_owner::c_ptr() const
{
    return p_struct;
}
mincgstate::mincgstate() : _mincgstate_owner() ,needf(p_struct->needf),needfg(p_struct->needfg),xupdated(p_struct->xupdated),f(p_struct->f),g(&p_struct->g),x(&p_struct->x)
{
}

mincgstate::mincgstate(alglib_impl::mincgstate *attach_to):_mincgstate_owner(attach_to) ,needf(p_struct->needf),needfg(p_struct->needfg),xupdated(p_struct->xupdated),f(p_struct->f),g(&p_struct->g),x(&p_struct->x)
{
}

mincgstate::mincgstate(const mincgstate &rhs):_mincgstate_owner(rhs) ,needf(p_struct->needf),needfg(p_struct->needfg),xupdated(p_struct->xupdated),f(p_struct->f),g(&p_struct->g),x(&p_struct->x)
{
}

mincgstate& mincgstate::operator=(const mincgstate &rhs)
{
    if( this==&rhs )
        return *this;
    _mincgstate_owner::operator=(rhs);
    return *this;
}

mincgstate::~mincgstate()
{
}


/*************************************************************************
This structure stores optimization report:
* IterationsCount           total number of inner iterations
* NFEV                      number of gradient evaluations
* TerminationType           termination type (see below)

TERMINATION CODES

TerminationType field contains completion code, which can be:
  -8    internal integrity control detected  infinite  or  NAN  values  in
        function/gradient. Abnormal termination signalled.
   1    relative function improvement is no more than EpsF.
   2    relative step is no more than EpsX.
   4    gradient norm is no more than EpsG
   5    MaxIts steps was taken
   7    stopping conditions are too stringent,
        further improvement is impossible,
        X contains best point found so far.
   8    terminated by user who called mincgrequesttermination(). X contains
        point which was "current accepted" when  termination  request  was
        submitted.

Other fields of this structure are not documented and should not be used!
*************************************************************************/
_mincgreport_owner::_mincgreport_owner()
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _state;
    
    alglib_impl::ae_state_init(&_state);
    if( setjmp(_break_jump) )
    {
        if( p_struct!=NULL )
        {
            alglib_impl::_mincgreport_destroy(p_struct);
            alglib_impl::ae_free(p_struct);
        }
        p_struct = NULL;
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_state.error_msg);
        return;
#endif
    }
    alglib_impl::ae_state_set_break_jump(&_state, &_break_jump);
    p_struct = NULL;
    p_struct = (alglib_impl::mincgreport*)alglib_impl::ae_malloc(sizeof(alglib_impl::mincgreport), &_state);
    memset(p_struct, 0, sizeof(alglib_impl::mincgreport));
    alglib_impl::_mincgreport_init(p_struct, &_state, ae_false);
    ae_state_clear(&_state);
    is_attached = false;
}

_mincgreport_owner::_mincgreport_owner(alglib_impl::mincgreport *attach_to)
{
    p_struct = attach_to;
    is_attached = true;
}

_mincgreport_owner::_mincgreport_owner(const _mincgreport_owner &rhs)
{
    jmp_buf _break_jump;
    alglib_impl::ae_state _state;
    
    alglib_impl::ae_state_init(&_state);
    if( setjmp(_break_jump) )
    {
        if( p_struct!=NULL )
        {
            alglib_impl::_mincgreport_destroy(p_struct);
            alglib_impl::ae_free(p_struct);
        }
        p_struct = NULL;
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_state.error_msg);
        return;
#endif
    }
    alglib_impl::ae_state_set_break_jump(&_state, &_break_jump);
    p_struct = NULL;
    alglib_impl::ae_assert(rhs.p_struct!=NULL, "ALGLIB: mincgreport copy constructor failure (source is not initialized)", &_state);
    p_struct = (alglib_impl::mincgreport*)alglib_impl::ae_malloc(sizeof(alglib_impl::mincgreport), &_state);
    memset(p_struct, 0, sizeof(alglib_impl::mincgreport));
    alglib_impl::_mincgreport_init_copy(p_struct, const_cast<alglib_impl::mincgreport*>(rhs.p_struct), &_state, ae_false);
    ae_state_clear(&_state);
    is_attached = false;
}

_mincgreport_owner& _mincgreport_owner::operator=(const _mincgreport_owner &rhs)
{
    if( this==&rhs )
        return *this;
    jmp_buf _break_jump;
    alglib_impl::ae_state _state;
    
    alglib_impl::ae_state_init(&_state);
    if( setjmp(_break_jump) )
    {
#if !defined(AE_NO_EXCEPTIONS)
        _ALGLIB_CPP_EXCEPTION(_state.error_msg);
#else
        _ALGLIB_SET_ERROR_FLAG(_state.error_msg);
        return *this;
#endif
    }
    alglib_impl::ae_state_set_break_jump(&_state, &_break_jump);
    alglib_impl::ae_assert(p_struct!=NULL, "ALGLIB: mincgreport assignment constructor failure (destination is not initialized)", &_state);
    alglib_impl::ae_assert(rhs.p_struct!=NULL, "ALGLIB: mincgreport assignment constructor failure (source is not initialized)", &_state);
    alglib_impl::ae_assert(!is_attached, "ALGLIB: mincgreport assignment constructor failure (can not assign to the structure which is attached to something else)", &_state);
    alglib_impl::_mincgreport_destroy(p_struct);
    memset(p_struct, 0, sizeof(alglib_impl::mincgreport));
    alglib_impl::_mincgreport_init_copy(p_struct, const_cast<alglib_impl::mincgreport*>(rhs.p_struct), &_state, ae_false);
    ae_state_clear(&_state);
    return *this;
}

_mincgreport_owner::~_mincgreport_owner()
{
    if( p_struct!=NULL && !is_attached )
    {
        alglib_impl::_mincgreport_destroy(p_struct);
        ae_free(p_struct);
    }
}

alglib_impl::mincgreport* _mincgreport_owner::c_ptr()
{
    return p_struct;
}

const alglib_impl::mincgreport* _mincgreport_owner::c_ptr() const
{
    return p_struct;
}
mincgreport::mincgreport() : _mincgreport_owner() ,iterationscount(p_struct->iterationscount),nfev(p_struct->nfev),terminationtype(p_struct->terminationtype)
{
}

mincgreport::mincgreport(alglib_impl::mincgreport *attach_to):_mincgreport_owner(attach_to) ,iterationscount(p_struct->iterationscount),nfev(p_struct->nfev),terminationtype(p_struct->terminationtype)
{
}

mincgreport::mincgreport(const mincgreport &rhs):_mincgreport_owner(rhs) ,iterationscount(p_struct->iterationscount),nfev(p_struct->nfev),terminationtype(p_struct->terminationtype)
{
}

mincgreport& mincgreport::operator=(const mincgreport &rhs)
{
    if( this==&rhs )
        return *this;
    _mincgreport_owner::operator=(rhs);
    return *this;
}

mincgreport::~mincgreport()
{
}