
ARCHEXP = /<archive.*?<\/archive>/m
RELEXP = /<archive.*?relative.*?=.*?["|'](.*?)['|"].*/
File.open(ARGV[0],"rb") { |f|
    
    root = "<archive>\n\n"
    
    res = f.read.scan(ARCHEXP)
    
    res.each{ |a|
        archivename = RELEXP.match(a)[1]
        if(archivename == "****phantomglobalvars****")
            puts "GLOBAL VARS USED TO BUILD THIS ARCHIVE:"
            puts "======================================="
            puts a
        else
            File.open(archivename,"wb") { |f2|
                f2 << a
            }
            root += "<include file='#{archivename}'/>\n"
        end
    }
    root += "\n\n</archive>\n\n"
    rootname = "root_"+ARGV[0]
    File.open(rootname,"wb") {|fr|
        fr << root
        puts "Call coolpkg on #{rootname}."
    }
}