from typing import Callable, Optional
from inspect import signature

from ._rvm._function import IBoolFunction, Function1, Function2, Function3, Function4, Function5

def Function(function: Callable[..., bool], name: Optional[str] = None, costs: float = 0.0) -> Callable[..., bool]:
    if name is None:
        name = getattr(function, '__name__', None)
        if name is None:
            raise ValueError("name is required when passing lambdas")

    sig = signature(function)

    argument_names = list(sig.parameters.keys())
    param_count = len(sig.parameters)

    function_type: IBoolFunction = None
    if param_count == 1:
        function_type = Function1
    elif param_count == 2:
        function_type = Function2
    elif param_count == 3:
        function_type = Function3
    elif param_count == 4:
        function_type = Function4
    elif param_count == 5:
        function_type = Function5
    else:
        raise NotImplementedError("only functions with up to 5 arguments are supported")

    return function_type(function, name, argument_names, costs)
