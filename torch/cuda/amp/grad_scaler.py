import torch

class GradScaler(torch.amp.GradScaler):
    r"""
    See :class:`torch.amp.GradScaler`.
    ``torch.cuda.amp.GradScaler(args...)`` is equivalent to ``torch.amp.GradScaler("cuda", args...)``
    """
    def __init__(self, init_scale=2.**16, growth_factor=2.0, backoff_factor=0.5,
                 growth_interval=2000, enabled=True):
        super().__init__("cuda", init_scale=init_scale, growth_factor=growth_factor, backoff_factor=backoff_factor,
                         growth_interval=growth_interval, enabled=enabled)
