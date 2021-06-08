doubleMe :: Int -> Int
doubleMe x = 2 * x

doubleUs :: Int -> Int -> Int
doubleUs x y = doubleMe x + doubleMe y

doubleSmallNumber :: Int -> Int
doubleSmallNumber x = if x > 100
    then x
    else doubleMe x
