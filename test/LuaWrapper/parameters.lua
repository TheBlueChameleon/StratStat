function echo(x, y)
    return x, y
end

function sumAndDifference(x, y)
    a, b = echo(x, y)
    sum = a + b
    difference = x - y
    return sum, difference
end
