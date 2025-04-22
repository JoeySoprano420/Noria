          [Instruction Issued]
                    |
        +-----------+-----------+
        |                       |
  [Directive Lookup]     [Opcode Bind Check]
        |                       |
  [Directive Class]         [Inline x64 Opcode]
        |                       |
  [Evaluate Inputs]        [Emit Binary Sequence]
        |
  [Trigger Outcome Generator]
        |
  [Push to Directive Stack]
        |
  [Outcome: Memory, Event, Flow]
        |
  [Checkpoint Validation] <----+
        |                     |
     [Continue]          [Rollback / Evade]
