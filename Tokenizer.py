token_specification = [
    ('START',    r'start'),
    ('END',      r'end'),
    ('ASSIGN',   r'assign'),
    ('PING',     r'ping'),
    ('IDENT',    r'[A-Za-z_][A-Za-z0-9_]*'),
    ('STRING',   r'"[^"]*"'),
    ('NUMBER',   r'\d+'),
    ('NEWLINE',  r'\n'),
    ('SKIP',     r'[ \t]+'),
    ('MISMATCH', r'.'),
]
