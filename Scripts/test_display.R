topos = c("full_mesh.py", "grid.py", "ring.py")
version = c("RIPng", "RIPng_ADD")
df2 <- data.frame(d = integer(), t = integer())
for (path in version) {
	for (topo in 1:3) {
		for (N in 1:10) {
			p = paste(c(path,topos[topo],N,"resume.out"), collapse='/')
			if (file.exists(p)) {
				df <- read.table(p, header = FALSE)
				for (i in 1:10) {
					df2[(N-1)*10+i, 1] = df[i,1]
					df2[(N-1)*10+i, 2] = N*N
				}
			} else {
				for (i in 1:10) {
	                                df2[(N-1)*10+i, 1] = 0
	                                df2[(N-1)*10+i, 2] = N*N
	
				}
			}
		}
		pdf(file = paste(c("~/resume", path, topos[topo], "pdf"), collapse='.'),   # The directory you want to save the file in
		    width = 4, # The width of the plot in inches
		    height = 4) # The height of the plot in inches
		df2.columns = c('d', 't')
		# Plot the chart.
		boxplot(d ~ t, data = df2, xlab = topos[topo],
		   ylab = "Nanoseconds to converge", main = paste("Time of convergence of", path))
	
		# Save the file.
		dev.off()
	}
}
