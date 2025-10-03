function identity(n)
--     print("Got table:")
--     for k,v in pairs(n) do
--         print(k.." = "..v)
--     end
--     print("...end")
    return(n)
end

function printTab(t)
    for k,v in pairs(t) do
        print(k.." = "..v)
    end
end
