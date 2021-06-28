import qualified Data.Map as Map

doubleMe :: Int -> Int
doubleMe x = 2 * x

doubleUs :: Int -> Int -> Int
doubleUs x y = doubleMe x + doubleMe y

doubleSmallNumber :: Int -> Int
doubleSmallNumber x = if x > 100
    then x
    else doubleMe x

showFirstTwo :: (Show a) => [a] -> String
showFirstTwo [] = "List is empty"
showFirstTwo (x:[]) = "List has one element, " ++ show x
showFirstTwo (x:y:[]) = "List has two elements, " ++ show x ++ " and " ++ show y
showFirstTwo (x:y:_) = "List has many elements, the first two are " ++ show x ++ " and " ++ show y

bmiTell :: (RealFloat a) => a -> a -> String  
bmiTell weight height  
    | bmi <= skinny = "You're underweight, you emo, you!"  
    | bmi <= normal = "You're supposedly normal. Pffft, I bet you're ugly!"  
    | bmi <= fat    = "You're fat! Lose some weight, fatty!"  
    | otherwise     = "You're a whale, congratulations!"  
    where
        bmi = weight / height ^ 2;
        skinny = 18.5;
        normal = 25.0;
        fat = 30.0

f = a + b
    where
        a = 2
        b = 3

quicksort :: (Ord a) => [a] -> [a]
quicksort [] = []
quicksort (x:xs) = (quicksort (filter (< x) xs)) ++ [x] ++ (quicksort (filter (>= x) xs))

(a, b) .* c = (a * c, b * c) -- Multiplication with scalar
multVecWithScalars :: Num a => (a, a) -> [a] -> (a, a)
multVecWithScalars = foldl (.*)


addTwoPolynomials :: Num a => [a] -> [a] -> [a]
addTwoPolynomials x y = zipWith (+) x y

addSeveralPolynomials :: Num a => [[a]] -> [a]
addSeveralPolynomials x = foldr1 addTwoPolynomials x

