(TeX-add-style-hook
 "LiteratureReview"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("xcolor" "table" "xcdraw")))
   (TeX-run-style-hooks
    "latex2e"
    "report"
    "rep10"
    "ctex"
    "geometry"
    "graphicx"
    "amsmath"
    "cite"
    "subcaption"
    "booktabs"
    "multirow"
    "xcolor")
   (LaTeX-add-labels
    "sec:label"
    "subsec:label"
    "fig:label"
    "tab:my-table")
   (LaTeX-add-bibliographies
    "references"))
 :latex)

