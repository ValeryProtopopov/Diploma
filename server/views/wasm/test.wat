(module
 (table 0 anyfunc)
 (memory $0 1)
 (data (i32.const 12) "\e8\03\00\00")
 (export "memory" (memory $0))
 (export "innerProduct" (func $innerProduct))
 (export "transposedMatrix" (func $transposedMatrix))
 (func $innerProduct (; 0 ;) (param $0 i32) (param $1 i32) (result f64)
  (local $2 i32)
  (local $3 f64)
  (set_local $3
   (f64.const 0)
  )
  (set_local $2
   (i32.const 0)
  )
  (loop $label$0
   (set_local $3
    (f64.add
     (get_local $3)
     (f64.mul
      (f64.load
       (i32.add
        (get_local $0)
        (get_local $2)
       )
      )
      (f64.load
       (i32.add
        (get_local $1)
        (get_local $2)
       )
      )
     )
    )
   )
   (br_if $label$0
    (i32.ne
     (tee_local $2
      (i32.add
       (get_local $2)
       (i32.const 8)
      )
     )
     (i32.const 8000)
    )
   )
  )
  (get_local $3)
 )
 (func $transposedMatrix (; 1 ;) (param $0 i32) (result f64)
  (f64.load
   (i32.load
    (get_local $0)
   )
  )
 )
)
