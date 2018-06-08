def tarai(x,y,z)
	if x<=y
		return y
	else
		tarai(tarai(x-1,y,z),tarai(y-1,z,x),tarai(z-1,x,y))
	end
end
puts tarai(12,6,0).to_s

