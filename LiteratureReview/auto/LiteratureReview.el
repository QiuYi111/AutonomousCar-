(TeX-add-style-hook
 "LiteratureReview"
 (lambda ()
   (TeX-run-style-hooks
    "latex2e"
    "paper"
    "paper10"
    "ctex"
    "geometry"
    "graphicx"
    "amsmath"
    "cite"
    "subcaption"
    "booktabs")
   (LaTeX-add-labels
    "sec:label"
    "subsec:label"
    "fig:label")
   (LaTeX-add-bibliographies
    "references"))
 :latex)

