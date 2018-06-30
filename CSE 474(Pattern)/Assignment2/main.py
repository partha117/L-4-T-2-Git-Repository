from TemplateMatching import  TemplateMatching



tem=TemplateMatching("ref5.jpeg","target5.png")
tem.match(Method=TemplateMatching.Match_Method_Hierarchical)
print(tem.get_ref_shape())
print(tem.get_target_shape())
