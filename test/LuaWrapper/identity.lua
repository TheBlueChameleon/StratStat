function identity(n)
    return(n)
end

function printTab(t)
    for k,v in pairs(t) do
        print(k.." = "..v)
    end
end
