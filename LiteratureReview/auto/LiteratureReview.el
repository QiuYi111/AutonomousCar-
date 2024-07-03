(TeX-add-style-hook
 "LiteratureReview"
 (lambda ()
   (TeX-run-style-hooks
    "latex2e"
    "ctexart"
    "ctexart10"
    "ctex"
    "graphicx"
    "amsmath"
    "cite"
    "booktabs")
   (LaTeX-add-labels
    "subsec:label"
    "sec:label")
   (LaTeX-add-bibliographies
    "references"))
 :latex)

