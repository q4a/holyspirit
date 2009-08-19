
main
	if alive
	then
		if distance * 8
		then
			if distance * 5 
			then
				if see
				then
					fight
				else
					if distance * 2
					then
						randomDisplace
					else
						followHero
					end
				end
			else
				followHero
			end
		else
			followHero
			teleport
		end
	end
end
